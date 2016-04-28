#pragma once

#include "Entity.h"
#include "glm.h"

class Camera : public Entity
{
    public:
        mat4 projectionMatrix;
        Camera(vec3 position);
        void configure(float fov, float aspect, float near, float far);
        void updateProjection(float aspect);

    private:
        float fov, aspect, near, far;
};

