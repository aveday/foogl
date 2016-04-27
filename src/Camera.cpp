#include "Camera.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 direction) :
    position(position), direction(direction)
{
    viewMatrix = glm::lookAt(
            position,
            position + direction,
            glm::vec3(0, 1, 0));
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

void Camera::updateProjection(float new_aspect)
{
    aspect = new_aspect;
    projectionMatrix = glm::perspective<GLfloat>(fov, aspect, near, far);
}
