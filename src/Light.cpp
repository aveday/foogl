#include "Light.h"
#include "glm.h"

Light::Light(GLuint shader, vec3 position, Color color) :
    Entity(shader, position, vec3()), color(color)
{
    updateShader(shader);
}

void Light::updateShader(GLuint shader)
{
    glUseProgram(shader);
    glUniform3f(
            glGetUniformLocation(shader, "lightPosition"),
            position.x, position.y, position.z);

    glUniform3f(
            glGetUniformLocation(shader, "lightColor"),
            color.x, color.y, color.z);
}
