#pragma once

#include "Entity.h"

class Terrain : public Entity
{
    public:
        Terrain(GLuint program, glm::vec3 pos);
        static int CreateModel();

};
