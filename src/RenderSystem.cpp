#include <glm/gtc/type_ptr.hpp>
#include "EntityManager.h"
#include "RenderSystem.h"

void RenderSystem::run(CameraC &camera, GLuint shader)
{
    glUseProgram(shader);
    // update the camera position uniform
    vec3 position(camera.transform[3]);//XXX not sure about this
    glUniform3fv(UNIFORM_CAMERA_POSITION, 1, glm::value_ptr(position));

    // calculate and set the camera matrix
    mat4 camera_matrix = camera.projection * glm::inverse(camera.transform);
    glUniformMatrix4fv(UNIFORM_CAMERA_MATRIX, 1, 0, glm::value_ptr(camera_matrix));

    for(int e = 0; e < EM::end(); e++) {
        if (!EM::has_components<ModelC>(e))
            continue;

        ModelC &model = EM::get_component<ModelC>(e);

        if (model.mesh->vertices_n == 0)
            continue;

        // update uniform shader inputs
        glUniformMatrix4fv(UNIFORM_MODEL, 1, 0, glm::value_ptr(model.transform));
        glUniform4fv(UNIFORM_COLOR, 1, glm::value_ptr(model.color));

        // bind vertex array and draw vertices
        glBindVertexArray(model.mesh->vao);
        glDrawArrays(GL_TRIANGLES, 0, model.mesh->vertices_n);
        glBindVertexArray(0);
    }
}

