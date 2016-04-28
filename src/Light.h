#pragma once

#include <GL/glew.h>

#include "glm.h"
#include "colors.h"

class Light
{
    public:
        Light(GLuint program, vec3 position, Color color = white);
        void move(vec3 diff);

    private:
        void updateShader();
        GLuint shaderProgram;
        vec3 position;
        vec3 color;
};

