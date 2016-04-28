#pragma once

#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>

#include "colors.h"

class Light
{
    public:
        Light(GLuint program, glm::vec3 position, Color color = white);
        void move(glm::vec3 diff);

    private:
        void updateShader();
        GLuint shaderProgram;
        glm::vec3 position;
        glm::vec3 color;
};

