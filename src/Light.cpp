#include "Light.h"

Light::Light(GLuint program, glm::vec3 position, Color color) :
    shaderProgram(program), position(position), color(color)
{
    glUseProgram(shaderProgram);
    glUniform3f(
            glGetUniformLocation(shaderProgram, "lightPosition"),
            position.x, position.y, position.z);

    glUniform3f(
            glGetUniformLocation(shaderProgram, "lightColor"),
            color.x, color.y, color.z);
}
