#include <glm/gtc/type_ptr.hpp>
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
