#pragma once

#include <GL/glew.h>

#include "Entity.h"
#include "glm.h"
#include "colors.h"

class Light : public Entity
{
    public:
        Light(GLuint shader, Mesh &mesh, vec3 position, Color color = white);
        void updateShader(GLuint shader);

    private:
        GLint offset[4];

        GLuint ubo;
        vec3 color;
        float attenuation = 0.2f;
        float ambientCoefficient = 0.005f;
};

