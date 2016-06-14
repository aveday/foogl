#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "EntityManager.h"
#include "LightSystem.h"
#include "components.h"

LightSystem::LightSystem()
{
    // Create the OpenGL buffer object
    glGenBuffers(1, &ubo);

    // set buffer size with glBufferData, bind the buffer to the uniform block
    GLint block_size = 16 + MAX_LIGHTS * 32;
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, block_size, nullptr, GL_DYNAMIC_DRAW );
    glBindBufferBase(GL_UNIFORM_BUFFER, UNIFORM_BINDING, ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    update_count(0);
}

void LightSystem::run()
{
    int n = 0;
    for(int e = 0; e < EM::end(); e++) {
        if ( !EM::has_components<Light, Body>(e) )
            continue;

        auto &position = EM::get_component<Body>(e).position;
        auto &intensity = EM::get_component<Light>(e).intensity;
        int offset = 16 + (n++) * 32;
        glBindBuffer(GL_UNIFORM_BUFFER, ubo); // FIXME std140 offsets need some tlc
        glBufferSubData(GL_UNIFORM_BUFFER, offset,    12, glm::value_ptr(position));
        glBufferSubData(GL_UNIFORM_BUFFER, offset+12, 4,  &attenuation);
        glBufferSubData(GL_UNIFORM_BUFFER, offset+16, 12, glm::value_ptr(intensity));
        glBufferSubData(GL_UNIFORM_BUFFER, offset+28, 4,  &ambientCoefficient);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    if(n != num_lights)
        update_count(n);
}

void LightSystem::update_count(int n) {
    num_lights = n;
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 4, &num_lights);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

