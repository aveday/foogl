#include <stdio.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "glm.h"

Camera::Camera(vec3 position, vec3 direction) :
    position(position), direction(direction)
{
    v_angle = 0.3f;
    h_angle = 3.2f;
    updateView();
    updateProjection(aspect);
}

void Camera::configure(float fov, float aspect, float near, float far)
{
    this->fov = fov;
    this->aspect = aspect;
    this->near = near;
    this->far = far;
    updateProjection(aspect);
}

void Camera::updateView()
{
    mat4 orientation;
    orientation = glm::rotate(orientation, v_angle, vec3(1,0,0));
    orientation = glm::rotate(orientation, h_angle, vec3(0,1,0));
    viewMatrix = orientation * glm::translate(mat4(), -position);
}

void Camera::turn(float v, float h) {
    v_angle += v;
    h_angle += h;
    updateView();
}

void Camera::updateProjection(float new_aspect)
{
    aspect = new_aspect;
    projectionMatrix = glm::perspective<GLfloat>(fov, aspect, near, far);
}
