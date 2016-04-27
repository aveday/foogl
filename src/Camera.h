#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Camera
{
    public:
        glm::mat4 projectionMatrix, viewMatrix;
        Camera(glm::vec3 position, glm::vec3 rotation);
        void configure(float fov, float aspect, float near, float far);
        void updateProjection(float aspect);
        void turn(float v, float h);
        void updateView();

    private:
        glm::vec3 position, direction;
        float fov, aspect, near, far;
        float h_angle, v_angle;
};

