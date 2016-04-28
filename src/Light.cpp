#include "Light.h"
#include "glm.h"

Light::Light(GLuint program, vec3 position, Color color) :
    shaderProgram(program), position(position), color(color)
{
    updateShader();
}

void Light::move(vec3 diff)
{
    position += diff;
    updateShader();
}

void Light::updateShader()
{
    glUseProgram(shaderProgram);
    glUniform3f(
            glGetUniformLocation(shaderProgram, "lightPosition"),
            position.x, position.y, position.z);

    glUniform3f(
            glGetUniformLocation(shaderProgram, "lightColor"),
            color.x, color.y, color.z);
}
