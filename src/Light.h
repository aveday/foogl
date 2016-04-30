#pragma once

#include <GL/glew.h>

#include "Entity.h"
#include "glm.h"
#include "colors.h"

class Light : public Entity
{
    public:
        Light(GLuint shader, vec3 position, Color color = white);

        void updateShader(GLuint shader);
    private:
        vec3 color;
};

