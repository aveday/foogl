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
    glUniform3f(
            glGetUniformLocation(shader, "cameraPosition"),
            position.x, position.y, position.z);

    // set the view matrix uniform to the inverse of the camera's model matrix
    mat4 cameraMatrix = projectionMatrix * glm::inverse( modelMatrix );
    glUniformMatrix4fv(
            glGetUniformLocation(shader, "camera"),
            1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::updateProjection(float new_aspect)
{
    aspect = new_aspect;
    projectionMatrix = glm::perspective<GLfloat>(fov, aspect, near, far);
}
