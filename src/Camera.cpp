#include <GL/glew.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "glm.h"

Camera::Camera(vec3 position) :
    Entity(0, position, vec3(1,1,1))
{
    rotation.x = 0;
    rotation.y = 180;
    updateModelMatrix();
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
