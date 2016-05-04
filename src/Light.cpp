#include <glm/gtc/type_ptr.hpp>
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
    float attenuation = 0.2f;
    float ambientCoefficient = 0.005f;

    glUseProgram(shader);
    glUniform3fv(
            glGetUniformLocation(shader, "light.position"),
            1, glm::value_ptr(position));

    glUniform3fv(
            glGetUniformLocation(shader, "light.color"),
            1, glm::value_ptr(color));

    glUniform1f(
            glGetUniformLocation(shader, "light.attenuation"),
            attenuation);

    glUniform1f(
            glGetUniformLocation(shader, "light.ambientCoefficient"),
            ambientCoefficient);
}
