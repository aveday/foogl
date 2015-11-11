#pragma once


#include "Entity.h"
#include "Perlin.h"

class Terrain : public Entity
{
    public:
        Terrain(GLuint program, int size, glm::vec3 pos);
        void CreateModel(int size, float seed_);
        int size;
        float seed;

    private:
        static Perlin perlin;
        float elevation = 2.0f;
        int levels = 3;
        float xScale[3] = {60, 30, 10};
        float yScale[3] = {40, 3, 2};
};
