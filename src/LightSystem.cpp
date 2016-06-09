#include <glm/gtc/type_ptr.hpp>
#include "LightSystem.h"

LightSystem::LightSystem(GLuint shader)
{
    GLuint block_index = glGetUniformBlockIndex(shader, "LIGHTS");

    // Create the OpenGL buffer object
    glGenBuffers(1, &ubo);

    // set buffer size with glBufferData, bind the buffer to the uniform block
    GLint block_size = 16 + MAX_LIGHTS * 32;
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, block_size, nullptr, GL_DYNAMIC_DRAW );
    glBindBufferBase(GL_UNIFORM_BUFFER, block_index, ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void LightSystem::run()
{
    int count = 0;
    for(int e = 0; e < EM::end(); e++) {
        if ( !EM::has_components<LightC>(e) )
            continue;

        auto &light = EM::get_component<LightC>(e);
        // std140 offsets need some tlc
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, 16+count*32,    12, glm::value_ptr(light.position));
        glBufferSubData(GL_UNIFORM_BUFFER, 16+count*32+12, 4,  &attenuation);
        glBufferSubData(GL_UNIFORM_BUFFER, 16+count*32+16, 12, glm::value_ptr(light.intensity));
        glBufferSubData(GL_UNIFORM_BUFFER, 16+count*32+28, 4,  &ambientCoefficient);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        ++count;
    }

    // update the shader light count
    if(count != num_lights) {
        num_lights = count;
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, 4, &num_lights);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}

