
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

Material AssetLoader::LoadMaterial(MaterialDef &def)
{
    Material material;
    material.diffuse = AL::LoadTexture(def.diffuse_filename);
    material.normal = AL::LoadTexture(def.normal_filename);
    return material;
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
    glVertexAttribPointer(INPUT_POSITION,  3, GL_FLOAT, 0, 56, (void*)0);
    glVertexAttribPointer(INPUT_NORMAL,    3, GL_FLOAT, 0, 56, (void*)12);
    glVertexAttribPointer(INPUT_TANGENT,   3, GL_FLOAT, 0, 56, (void*)24);
    glVertexAttribPointer(INPUT_BITANGENT, 3, GL_FLOAT, 0, 56, (void*)36);
    glVertexAttribPointer(INPUT_UV,        2, GL_FLOAT, 0, 56, (void*)48);

    glEnableVertexAttribArray(INPUT_POSITION);
    glEnableVertexAttribArray(INPUT_NORMAL);
    glEnableVertexAttribArray(INPUT_TANGENT);
    glEnableVertexAttribArray(INPUT_BITANGENT);
    glEnableVertexAttribArray(INPUT_UV);

    struct Vertex { vec3 position, normal, tangent, bitangent; vec2 tex_coord; };
    Vertex *vertices = new Vertex[mesh.vertices_n];

    // set vertex positions and normals
    for (int i = 0; i < mesh.vertices_n; i++) {
        vertices[i].position = def.positions[def.indices[i]];
        vertices[i].normal =
            def.normal_type == SURFACE_NORMALS  ? def.normals[i/3] :
            def.normal_type == POSITION_NORMALS ? def.normals[def.indices[i]] :
            def.normal_type == VERTEX_NORMALS   ? def.normals[i] :
            def.normal_type == NO_NORMALS       ? vec3{0,0,0} : vec3{0,0,1};
    }

    // calculate texture coordinates
    vec3 tex_normal, axis;
    vec2 tpos;
    float angle;
    for(int i = 0; i < mesh.vertices_n; i++) {
        // set tpos by position for projected textures
        if (def.texture_type == PROJECTED) {
            tpos = vec2( vertices[i].position );

        // otherwise find tpos by rotating point to XY plane
        } else {
            // first get the normal of the point
            if (def.texture_type == STRETCHED) {
                if (def.normal_type != POSITION_NORMALS)
                    std::cerr << "STRETCHED requires POSITION_NORMALS\n";
                tex_normal = vertices[i].normal;
            } else if (i%3 == 0) {
                tex_normal = (def.normal_type == SURFACE_NORMALS)
                    ? vertices[i].normal
                    : get_normal( vertices[i  ].position,
                                  vertices[i+1].position,
                                  vertices[i+2].position );
            }
            // define the rotation axis depenging on the normal
            if (def.texture_type == STRETCHED || i%3 == 0) {
                angle = glm::angle(tex_normal, {0,0,1});
                axis = glm::cross(tex_normal, {0,0,1});
                if(axis == vec3(0)) axis = {1,0,0};
            }
            // rotate the point position to find the texture position
            tpos = vec2( glm::rotate(vertices[i].position, angle, axis) );
        }
        // scale and center the final texture coordinates
        vertices[i].tex_coord = def.texture_scale * tpos + vec2(.5);
    }

    for (int i = 0; i < mesh.vertices_n; i++) {
        int n = i - i%3;
        //printf("1\n");
        vec3 &v0 = vertices[n+0].position;
        vec3 &v1 = vertices[n+1].position;
        vec3 &v2 = vertices[n+2].position;

        //printf("2\n");
        vec2 &uv0 = vertices[n+0].tex_coord;
        vec2 &uv1 = vertices[n+1].tex_coord;
        vec2 &uv2 = vertices[n+2].tex_coord;

        vec3 deltaPos1 = v1-v0;
        vec3 deltaPos2 = v2-v0;
        // UV delta
        vec2 deltaUV1 = uv1-uv0;
        vec2 deltaUV2 = uv2-uv0;

        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        vec3 normal = vertices[i].normal;
        vec3 tangent = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y)*r;
        vec3 bitangent = (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x)*r;


        vertices[i+0].tangent = tangent;
        vertices[i+1].tangent = tangent;
        vertices[i+2].tangent = tangent;

        vertices[i+0].bitangent = bitangent;
        vertices[i+1].bitangent = bitangent;
        vertices[i+2].bitangent = bitangent;
    }

	// See "Going Further"
	for (int i=0; i < mesh.vertices_n; i++) {
		glm::vec3 &n = vertices[i].normal;
		glm::vec3 &t = vertices[i].tangent;
		glm::vec3 &b = vertices[i].bitangent;
		
		// Gram-Schmidt orthogonalize
		t = glm::normalize(t - n * glm::dot(n, t));
		
		// Calculate handedness
		if (glm::dot(glm::cross(n, t), b) < 0.0f){
			t = t * -1.0f;
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

