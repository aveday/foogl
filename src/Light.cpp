#include <glm/gtc/type_ptr.hpp>
#include <cstdio>
#include "Light.h"
#include "glm.h"

int Light::count = 0;
GLint Light::count_offset = -1;
GLuint Light::ubo;

Light::Light(GLuint shader, Mesh &mesh, vec3 position, vec3 intensity) :
    Entity(mesh, position, vec3(.01)), intensity(intensity)
{
    // get count_offset and create buffer for first light
    if(count == 0)
    {
        GLuint count_index;
        const GLchar *names[] = {"num_lights"};
        glGetUniformIndices(shader, 1, names, &count_index);
        glGetActiveUniformsiv(shader, 1, &count_index, GL_UNIFORM_OFFSET, &count_offset);

        // get the index of the uniform block
        GLuint block_index = glGetUniformBlockIndex(shader, "UBO");
        // find the size required for the buffer object
        GLint block_size;
        glGetActiveUniformBlockiv(shader, block_index,
                GL_UNIFORM_BLOCK_DATA_SIZE, &block_size);

        // Create the OpenGL buffer object
        glGenBuffers(1, &ubo);

        // set buffer size with glBufferData, bind the buffer to the uniform block
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferData(GL_UNIFORM_BUFFER, block_size, nullptr, GL_DYNAMIC_DRAW );
        glBindBufferBase(GL_UNIFORM_BUFFER, block_index, ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    // build the uniform names //TODO consider redoing all this with std140
    char buffer[4][50];
    sprintf(buffer[0], "lights[%d].position", count);
    sprintf(buffer[1], "lights[%d].intensity", count);
    sprintf(buffer[2], "lights[%d].attenuation", count);
    sprintf(buffer[3], "lights[%d].ambientCoefficient", count);
    const GLchar *names[] = { buffer[0], buffer[1], buffer[2], buffer[3]};

    // find the offsets of each block variable
    GLuint indices[4];
    glGetUniformIndices(shader, 4, names, indices);
    glGetActiveUniformsiv(shader, 4, indices, GL_UNIFORM_OFFSET, offset);

    // increment light count and update num_lights in shader
    count++;
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, count_offset, sizeof(GLint), &count);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

// update light uniforms
void Light::updateShader(GLuint shader)
{
<<<<<<< HEAD
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
=======
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, offset[0], 3*sizeof(GLfloat), glm::value_ptr(position));
    glBufferSubData(GL_UNIFORM_BUFFER, offset[1], 3*sizeof(GLfloat), glm::value_ptr(intensity));
    glBufferSubData(GL_UNIFORM_BUFFER, offset[2], 1*sizeof(GLfloat), &attenuation);
    glBufferSubData(GL_UNIFORM_BUFFER, offset[3], 1*sizeof(GLfloat), &ambientCoefficient);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
>>>>>>> Replace shader class with AL, Fix UBO binding
}
