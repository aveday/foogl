
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <GL/glew.h>
#include "AssetLoader.h"
#include "glm.h"

std::unordered_map<std::string, GLuint> AssetLoader::program_cache;
std::unordered_map<std::string, Texture> AssetLoader::texture_cache;
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

    mesh_cache[&def] = Mesh{def.vertices_n};
    Mesh &mesh = mesh_cache[&def];

    // create and bind the vertex buffer
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

    // create and bind the vertex array
    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    // define vertex buffer layout
    glVertexAttribPointer(INPUT_POSITION, 3, GL_FLOAT, 0, 24, (void*)0);
    glEnableVertexAttribArray(INPUT_POSITION);
    glVertexAttribPointer(INPUT_NORMAL,   3, GL_FLOAT, 0, 24, (void*)12);
    glEnableVertexAttribArray(INPUT_NORMAL);

    // buffer vertices
    vec3* vertices = new vec3[2*mesh.vertices_n];
    for(int i = 0; i < mesh.vertices_n; i++) {
        vertices[2*i] = def.positions[def.indices[i]];
        vertices[2*i+1] = def.normals[i/6];
    }
    int size = 2*mesh.vertices_n*sizeof(vec3);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    delete [] vertices;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return mesh;
}

Texture AssetLoader::LoadTexture(std::string file)
{
    // check if texture has been cached
    if (texture_cache.count(file))
        return texture_cache[file];

    // load and check the image file with stb_image
    int width, height, channels;
    uint8_t* pixels = stbi_load(file.c_str(), &width, &height, &channels, 0);
    if(!pixels) throw std::runtime_error(stbi_failure_reason());

    // cache and return the new texture
    Texture texture{width, height, channels, pixels};
    texture_cache[file] = texture;
    return texture;
}
