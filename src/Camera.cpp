#include "Camera.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(float fov, float aspect, float near, float far) :
    fov(fov), aspect(aspect), near(near), far(far)
{
    viewMatrix = glm::lookAt(
            glm::vec3(0, 5, -10),
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0));
    updateProjection(aspect);
}

void Camera::updateProjection(float new_aspect)
{
    aspect = new_aspect;
    projectionMatrix = glm::perspective<GLfloat>(fov, aspect, near, far);
}
