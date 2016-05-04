#pragma once

#include <GL/glew.h>

#include "Entity.h"
#include "glm.h"
#include "colors.h"

class Light : public Entity
{
    public:
        Light(GLuint shader, Mesh &mesh, vec3 position, Color color = white);
        ~Light() { count--; }
        void updateShader(GLuint shader);

    private:
        static const int MAX_LIGHTS = 10;
        static int count;
        static GLint count_offset;
        GLint offset[4];

        GLuint ubo;
        vec3 color;
        float attenuation = 0.2f;
        float ambientCoefficient = 0.005f;
};

