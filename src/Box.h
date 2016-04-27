#pragma once

#include "Entity.h"

class Box : public Entity
{
    public:
        Box(GLuint program, glm::vec3 size_, glm::vec3 pos, glm::vec4 color);
        glm::vec4 color;

};

