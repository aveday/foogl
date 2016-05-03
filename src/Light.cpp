#include <glm/gtc/type_ptr.hpp>
#include <string.h>
#include "Light.h"
#include "glm.h"

Light::Light(GLuint shader, Mesh &mesh, vec3 position, Color color) :
    Entity(mesh, position, vec3(.01)), color(color)
{
    // get the index of the uniform block
    GLuint blockIndex = glGetUniformBlockIndex(shader, "Light");

    // find the offsets of each block variable
    const GLchar *names[] = {
        "Light.position",
        "Light.color",
        "Light.attenuation",
        "Light.ambientCoefficient"
    };
    GLuint indices[4];
    glGetUniformIndices(shader, 4, names, indices);
    glGetActiveUniformsiv(shader, 4, indices, GL_UNIFORM_OFFSET, offset);

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

void Light::updateShader(GLuint shader)
{
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);

    glBufferSubData(GL_UNIFORM_BUFFER, offset[0], 3*sizeof(GLfloat), glm::value_ptr(position));
    glBufferSubData(GL_UNIFORM_BUFFER, offset[1], 3*sizeof(GLfloat), glm::value_ptr(color));
    glBufferSubData(GL_UNIFORM_BUFFER, offset[2], 1*sizeof(GLfloat), &attenuation);
    glBufferSubData(GL_UNIFORM_BUFFER, offset[3], 1*sizeof(GLfloat), &ambientCoefficient);
}
