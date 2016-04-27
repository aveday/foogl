#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Camera
{
    public:
        glm::mat4 projectionMatrix, viewMatrix;

        Camera(float fov, float aspect, float near, float far);
        void updateProjection(float aspect);

    private:
        float fov, aspect, near, far;
};

