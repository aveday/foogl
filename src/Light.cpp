#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include "Light.h"
#include "glm.h"

int Light::count = 0;
GLint Light::count_offset = -1;

Light::Light(GLuint shader, Mesh &mesh, vec3 position, Color color) :
    Entity(mesh, position, vec3(.01)), color(color)
{
    // get count_offset and create buffer for first light
    if(count == 0)
    {
        // get the index of the uniform block
        GLuint blockIndex = glGetUniformBlockIndex(shader, "UBO");

        GLuint count_index;
        const GLchar *names[] = {"num_lights"};
        glGetUniformIndices(shader, 1, names, &count_index);
        glGetActiveUniformsiv(shader, 1, &count_index, GL_UNIFORM_OFFSET, &count_offset);

        // find the size required for the buffer object
        GLint blockSize;
        glGetActiveUniformBlockiv(shader, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

        // Create the OpenGL buffer object
        glGenBuffers(1, &ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferData(GL_UNIFORM_BUFFER, blockSize, nullptr, GL_DYNAMIC_DRAW );

        // bind the buffer to the uniform block
        glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, ubo);
    }

    // build the uniform names //TODO consider redoing all this with std140
    char buffer[4][50];
    sprintf(buffer[0], "lights[%d].position", count);
    sprintf(buffer[1], "lights[%d].color", count);
    sprintf(buffer[2], "lights[%d].attenuation", count);
    sprintf(buffer[3], "lights[%d].ambientCoefficient", count);
    const GLchar *names[] = { buffer[0], buffer[1], buffer[2], buffer[3]};

    // find the offsets of each block variable
    GLuint indices[4];
    glGetUniformIndices(shader, 4, names, indices);
    glGetActiveUniformsiv(shader, 4, indices, GL_UNIFORM_OFFSET, offset);

    // increment light count and update num_lights in shader
    count++;
    glBufferSubData(GL_UNIFORM_BUFFER, count_offset, sizeof(GLint), &count);
}

void Light::updateShader(GLuint shader)
{
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    // update light uniforms
    glBufferSubData(GL_UNIFORM_BUFFER, offset[0], 3*sizeof(GLfloat), glm::value_ptr(position));
    glBufferSubData(GL_UNIFORM_BUFFER, offset[1], 3*sizeof(GLfloat), glm::value_ptr(color));
    glBufferSubData(GL_UNIFORM_BUFFER, offset[2], 1*sizeof(GLfloat), &attenuation);
    glBufferSubData(GL_UNIFORM_BUFFER, offset[3], 1*sizeof(GLfloat), &ambientCoefficient);
}
