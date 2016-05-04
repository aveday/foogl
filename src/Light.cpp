#include <glm/gtc/type_ptr.hpp>
#include "Light.h"
#include "glm.h"

Light::Light(Mesh &mesh, vec3 position, Color color) :
    Entity(mesh, position, vec3(.01)), color(color)
{
}

void Light::updateShader(GLuint shader)
{
    float attenuation = 0.2f;
    float ambientCoefficient = 0.005f;

    glUseProgram(shader);
    glUniform3fv(
            glGetUniformLocation(shader, "light.position"),
            1, glm::value_ptr(position));

    glUniform3fv(
            glGetUniformLocation(shader, "light.color"),
            1, glm::value_ptr(color));

    glUniform1f(
            glGetUniformLocation(shader, "light.attenuation"),
            attenuation);

    glUniform1f(
            glGetUniformLocation(shader, "light.ambientCoefficient"),
            ambientCoefficient);
}
