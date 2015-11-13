#pragma once


#include "Entity.h"
#include "Perlin.h"

class Terrain : public Entity
{
    public:
        Terrain(GLuint program, int size, glm::vec3 pos);
        void CreateModel();
        int size = 64;
        float seed = 0;
        float scale = 0.2f;

        int octaves = 4;
        float baseFreq = 0.02f, baseAmp= 40.0f;
        float lac = 4.0f, pers = 0.20f;
    private:
        static Perlin perlin;
        float elevation = 2.0f;
        glm::vec2 gridToWorld(int x, int y, int size);
};
