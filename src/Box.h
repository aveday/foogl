#pragma once

#include "Entity.h"
#include "glm.h"

class Box : public Entity
{
    public:
        Box(GLuint program, vec3 size_, vec3 pos, vec4 color);
        vec4 color;

};

