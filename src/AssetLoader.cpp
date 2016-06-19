
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <GL/glew.h>
#include "AssetLoader.h"
#include "glm.h"

std::unordered_map<std::string, GLuint> AssetLoader::program_cache;
std::unordered_map<std::string, GLuint> AssetLoader::texture_cache;
std::unordered_map<MeshDef*, Mesh> AssetLoader::mesh_cache;

/* Load and compile a shader given a shader type and filename */
GLuint AssetLoader::LoadShader(GLenum type, const char *filename)
{
    // open the shader source
    FILE *f = fopen(filename, "r");
    if (!f) {
        std::cerr << "Unable to open " << filename << " for reading";
        return 0;
    }

    // read the source
    fseek(f, 0, SEEK_END);
    int length = ftell(f);
    fseek(f, 0, SEEK_SET);
    GLchar *source = (char*)malloc(length+1);
    length = fread(source, 1, length, f);
    fclose(f);
    ((char*)source)[length] = '\0';

    // compile the shader
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar**)&source, &length);
    free(source);
    glCompileShader(shader);

    // check the shader
    GLint shader_ok;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
    if (!shader_ok) {
        GLint log_length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
        char *log = (char*)malloc(log_length);
        glGetShaderInfoLog(shader, log_length, NULL, log);
        std::cerr << "Failed to compile " << filename << ".\n" << log;
        free(log);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

/* Load a shader program given vertex and fragment shader filenames */
GLuint AssetLoader::LoadProgram(const char* vs, const char* fs)
{
    // attempt to load the program from the cache
    // FIXME maybe this shouldn't be done with strings
    std::string cache_index = std::string(vs) + ":" + std::string(fs);
    if (program_cache.count(cache_index))
        return program_cache[cache_index];

    // create shader program, compile and attach vertex and fragment shaders
    GLuint program = glCreateProgram();
    glAttachShader(program, LoadShader(GL_VERTEX_SHADER, vs));
    glAttachShader(program, LoadShader(GL_FRAGMENT_SHADER, fs));

    // link and check the program
    glLinkProgram(program);
    GLint program_ok;
    glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
    if (!program_ok) {
        GLint log_length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
        char *log = (char*)malloc(log_length);
        glGetShaderInfoLog(program, log_length, NULL, log);
        std::cerr << "Error linking the shader program.\n" << log;
        free(log);
        glDeleteProgram(program);
    }

    // validate and cache new program
    glValidateProgram(program);
    program_cache[cache_index] = program;

    return program;
}

Mesh AssetLoader::LoadMesh(MeshDef &def)
{
    if (mesh_cache.count(&def))
        return mesh_cache[&def];

    mesh_cache[&def] = Mesh{(int)def.indices.size()};
    Mesh &mesh = mesh_cache[&def];

    // create and bind the vertex buffer
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

    // create and bind the vertex array
    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    // define vertex buffer layout
    glVertexAttribPointer(INPUT_POSITION, 3, GL_FLOAT, 0, 32, (void*)0);
    glVertexAttribPointer(INPUT_NORMAL,   3, GL_FLOAT, 0, 32, (void*)12);
    glVertexAttribPointer(INPUT_TEXCOORD, 2, GL_FLOAT, 0, 32, (void*)24);
    glEnableVertexAttribArray(INPUT_POSITION);
    glEnableVertexAttribArray(INPUT_NORMAL);
    glEnableVertexAttribArray(INPUT_TEXCOORD);

    struct Vertex { vec3 position, normal; vec2 tex_coord; };
    Vertex *vertices = new Vertex[mesh.vertices_n];

    // set vertex positions and normals
    for (int i = 0; i < mesh.vertices_n; i++)
        vertices[i].position = def.positions[def.indices[i]];

    for (int i = 0; i < mesh.vertices_n; i+=3) {
        for (int n = 0; n < 3; n++)
            vertices[i+n].normal = def.normals[i/3];
    }

    // calculate texture coordinate by rotating to z-plane
    for(int i = 0; i < mesh.vertices_n; i+=3) {
        float angle = glm::angle(vertices[i].normal, {0,0,1});
        vec3 normal = glm::cross(vertices[i].normal, {0,0,1});
        if(normal == vec3(0)) normal = {1,0,0};
        for (int n = 0; n < 3; n++) {
            vec3 tpos = glm::rotate(vertices[i+n].position, angle, normal);
            vertices[i+n].tex_coord = { tpos.x + 0.5, tpos.y + 0.5 };
        }
    }

    // buffer vertex data
    int size = sizeof(Vertex) * mesh.vertices_n;
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    delete [] vertices;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return mesh;
}

GLuint AssetLoader::LoadTexture(std::string filename)
{
    std::string file = ASSET_DIR + filename;

    // check if texture has been cached
    if (texture_cache.count(file))
        return texture_cache[file];

    // load and check the image file with stb_image
    int width, height, channels;
    uint8_t* pixels = stbi_load(file.c_str(), &width, &height, &channels, 0);
    if(!pixels) throw std::runtime_error(stbi_failure_reason());

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, pixels);

    stbi_image_free(pixels);
    glBindTexture(GL_TEXTURE_2D, 0);

    // cache and return the new texture
    texture_cache[file] = texture;
    return texture;
}

