#include <GL/glew.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "glm.h"

Camera::Camera(Mesh &mesh, vec3 position) :
    Entity(mesh, position, vec3(1)),
    fov(45),
    aspect(16/9.0f),//FIXME
    near(0.1f),
    far(1000.0f)
{
    rotation.x = 0;
    rotation.y = 0;
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

void Camera::updateShader(GLuint shader)
{
    // update the camera position uniform
    glUniform3f(UNIFORM_POSITION, position.x, position.y, position.z);

    // calculate and set the camera matrix from the view and projection
    mat4 cameraMatrix = projectionMatrix * glm::inverse( modelMatrix );
    glUniformMatrix4fv(UNIFORM_MATRIX, 1, 0, glm::value_ptr(cameraMatrix));
}

void Camera::updateProjection(float new_aspect)
{
    aspect = new_aspect;
    projectionMatrix = glm::perspective<GLfloat>(fov, aspect, near, far);
}
