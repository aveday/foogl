#pragma once

#include "Entity.h"
#include "glm.h"

class Camera : public Entity
{
    public:
        mat4 projectionMatrix;
        Camera(Mesh &mesh, vec3 position);
        void configure(float fov, float aspect, float near, float far);
        void updateShader(GLuint shader);
        void updateProjection(float aspect);

    private:
        float fov, aspect, near, far;

        static const GLuint UNIFORM_MATRIX = 0;
        static const GLuint UNIFORM_POSITION = 3;
};

