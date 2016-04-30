#include "Light.h"
#include "glm.h"

Light::Light(Mesh &mesh, vec3 position, Color color) :
    Entity(mesh, position, vec3(.1)), color(color)
{
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
