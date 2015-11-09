#pragma once

#include "Entity.h"
#include "Mesh.h"
#include "Perlin.h"

class Terrain : public Entity
{
    public:
        Terrain(GLuint program, glm::vec3 pos);
        void CreateModel();

    private:
        int size = 128;
        float elevation = 2.0f;

        Perlin perlin = Perlin(103);
        int levels = 3;
        float xScale[3] = {0.6f, 0.3f, 0.1f};
        float yScale[3] = {0.4f, 0.03f, 0.02f};


};
