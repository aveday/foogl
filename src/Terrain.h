#pragma once

#include <iostream>

#include "Entity.h"
#include "Mesh.h"
#include "Perlin.h"

class Terrain : public Entity
{
    public:
        Terrain(GLuint program, int size, glm::vec3 pos);
        void CreateModel(int size);
        int size;

        float elevation = 2.0f;

};
