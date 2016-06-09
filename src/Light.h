#pragma once

#include <GL/glew.h>

#include "Entity.h"
#include "glm.h"
#include "colors.h"

class Light : public Entity
{
    public:
        Light(GLuint shader, Mesh &mesh, vec3 position, vec3 intensity);
        ~Light() { count--; }
        void updateLighting(int light_num);

    private:
        static const int MAX_LIGHTS = 10;
        static int count;
        static GLuint ubo;

        vec3 intensity;
        float attenuation = 0.2f;
        float ambientCoefficient = 0.005f;
};
