#include <glm/gtc/type_ptr.hpp>
#include <cstdio>
#include "Light.h"
#include "glm.h"

int Light::count = 0;
GLuint Light::ubo;

Light::Light(GLuint shader, Mesh &mesh, vec3 position, vec3 intensity) :
    Entity(mesh, position, vec3(.01)), intensity(intensity)
{
    // get count_offset and create buffer for first light
    if(count == 0)
    {
        GLuint block_index = glGetUniformBlockIndex(shader, "LIGHTS");

        // Create the OpenGL buffer object
        glGenBuffers(1, &ubo);

        GLint block_size = 16 + MAX_LIGHTS * 32;
        // set buffer size with glBufferData, bind the buffer to the uniform block
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferData(GL_UNIFORM_BUFFER, block_size, nullptr, GL_DYNAMIC_DRAW );
        glBindBufferBase(GL_UNIFORM_BUFFER, block_index, ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    // increment light count and update num_lights in shader
    count++;
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GLint), &count);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

// update light uniforms
void Light::updateLighting(int n)
{
    // std140 offsets need some tlc
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 16+n*32,    12, glm::value_ptr(position));
    glBufferSubData(GL_UNIFORM_BUFFER, 16+n*32+12, 4,  &attenuation);
    glBufferSubData(GL_UNIFORM_BUFFER, 16+n*32+16, 12, glm::value_ptr(intensity));
    glBufferSubData(GL_UNIFORM_BUFFER, 16+n*32+28, 4,  &ambientCoefficient);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
