#pragma once

#include "glm.h"

class Camera
{
    public:
        mat4 projectionMatrix, viewMatrix;
        Camera(vec3 position, vec3 rotation);
        void configure(float fov, float aspect, float near, float far);
        void updateProjection(float aspect);
        void turn(float v, float h);
        void updateView();

    private:
        vec3 position, direction;
        float fov, aspect, near, far;
        float h_angle, v_angle;
};

