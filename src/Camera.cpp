#include <GL/glew.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "glm.h"

Camera::Camera(vec3 position) :
    Entity(0, position, vec3(1,1,1)),
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
    glUniform3f(
            glGetUniformLocation(shader, "cameraPosition"),
            position.x, position.y, position.z);

    mat4 viewMatrix = glm::inverse( modelMatrix );
    glUniformMatrix4fv(
            glGetUniformLocation(shader, "view"),
            1, GL_FALSE, glm::value_ptr(viewMatrix));

    glUniformMatrix4fv(
            glGetUniformLocation(shader, "projection"),
            1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void Camera::updateProjection(float new_aspect)
{
    aspect = new_aspect;
    projectionMatrix = glm::perspective<GLfloat>(fov, aspect, near, far);
}
