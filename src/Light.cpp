#include <glm/gtc/type_ptr.hpp>
#include <string.h>
#include "Light.h"
#include "glm.h"

Light::Light(GLuint shader, Mesh &mesh, vec3 position, Color color) :
    Entity(mesh, position, vec3(.01)), color(color)
{
    // get the uniform block index
    GLuint index = glGetUniformBlockIndex(shader, "Light");

    // find the size required
    GLint blockSize;
    glGetActiveUniformBlockiv(shader, index, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
 
    // create the buffer
    GLubyte* blockBuffer = (GLubyte*)malloc(blockSize);

    // query for the offsets of each block variable
    const GLchar *names[] = {
        "lightposition",
        "lightcolor",
        "lightattenuation",
        "lightambientCoefficient"
    };
    
    GLuint indices[4];
    glGetUniformIndices(shader, 4, names, indices);

    GLint offset[4];
    glGetActiveUniformsiv(shader, 4, indices, GL_UNIFORM_OFFSET, offset);

    // write the data to the buffer
    memcpy(blockBuffer + offset[0], glm::value_ptr(position), 3 * sizeof(GLfloat));
    memcpy(blockBuffer + offset[1], glm::value_ptr(color), 3 * sizeof(GLfloat));
    memcpy(blockBuffer + offset[2], &attenuation, sizeof(GLfloat));
    memcpy(blockBuffer + offset[3], &ambientCoefficient, sizeof(GLfloat));

    // Create the OpenGL buffer object and copy the data into it.
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer, GL_DYNAMIC_DRAW );

    // bind the buffer
    glBindBufferBase(GL_UNIFORM_BUFFER, index, ubo);
}

void Light::updateShader(GLuint shader)
{
    glUseProgram(shader);


    /*
    glUniform3f(
            glGetUniformLocation(shader, "light.position"),
            position.x, position.y, position.z);

    glUniform3f(
            glGetUniformLocation(shader, "light.color"),
            color.x, color.y, color.z);

    glUniform1f(
            glGetUniformLocation(shader, "light.attenuation"),
            attenuation);

    glUniform1f(
            glGetUniformLocation(shader, "light.ambientCoefficient"),
            ambientCoefficient);
    */
}
