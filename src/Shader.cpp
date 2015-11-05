#include <GL/glew.h>
#include <GL/glu.h>

#include <stdlib.h>
#include <stdio.h>

#include "Shader.h"

/* Load and compile a shader given a shader type and filename */
GLuint loadShader(GLenum type, const char *filename)
{
    // open the shader source
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        fprintf(stderr, "Unable to open %s for reading\n", filename);
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
        fprintf(stderr, "Failed to compile %s:\n%s", filename, log);
        free(log);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

/* Create a shader program givan a vertex shader filename and fragment shader filename */
GLuint loadProgram(const char *vsName, const char *fsName)
{
    // create the shader program
    GLuint program = glCreateProgram();

    // compile and attach the vertex and fragment shaders
    glAttachShader(program, loadShader(GL_VERTEX_SHADER, vsName));
    glAttachShader(program, loadShader(GL_FRAGMENT_SHADER, fsName));
    
    // link the program
    glLinkProgram(program);

    // check the program
    GLint program_ok;
    glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
    if (!program_ok) {
        GLint log_length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
        char *log = (char*)malloc(log_length);
        glGetShaderInfoLog(program, log_length, NULL, log);
        fprintf(stderr, "Error linking shader program:\n%s", log);
        free(log);
        glDeleteProgram(program);
    }
    glValidateProgram(program);

    return program;
}
