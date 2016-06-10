#include <cstdio>
#include <iostream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "glm.h"
#include "AssetLoader.h"

std::unordered_map<std::string, GLuint> AssetLoader::program_cache;
std::unordered_map<MeshDef*, Mesh> AssetLoader::mesh_cache;

template <typename C> std::vector<C> AssetLoader::asset_vector;

/* Load and compile a shader given a shader type and filename */
GLuint AssetLoader::LoadShader(GLenum type, const char *filename)
{
    // open the shader source
    FILE *f = fopen(filename, "r");
    if (!f)
    {
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
    if (!shader_ok)
    {
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

Mesh& AssetLoader::LoadMesh(MeshDef &def)
{
    if (!mesh_cache.count(&def))
        mesh_cache[&def] = Mesh(def);

    return mesh_cache[&def];
}


