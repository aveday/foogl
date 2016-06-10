#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "EntityManager.h"
#include "RenderSystem.h"
#include "Component.h"

void RenderSystem::run(int viewer)
{
    if (!EM::has_components<Camera>(viewer)) {
        std::cerr << "RenderSystem viewer needs Camera component\n";
        exit(EXIT_FAILURE);
    }
    Camera &camera = EM::get_component<Camera>(viewer);
    Transform &transform = EM::get_component<Transform>(viewer);

    // update the camera position uniform
    vec3 position(transform[3]);//XXX not sure about this
    glUniform3fv(UNIFORM_VIEW_POSITION, 1, glm::value_ptr(position));

    // calculate and set the view matrix
    mat4 view_matrix = camera.projection * glm::inverse(transform);
    glUniformMatrix4fv(UNIFORM_VIEW_MATRIX, 1, 0, glm::value_ptr(view_matrix));

    for(int e = 0; e < EM::end(); e++) {
        if (!EM::has_components<Model>(e))
            continue;

        Model &model = EM::get_component<Model>(e);
        Transform &transform = EM::get_component<Transform>(e);

        if (model.mesh->vertices_n == 0)
            continue;

        // update uniform shader inputs
        glUniformMatrix4fv(UNIFORM_MODEL, 1, 0, glm::value_ptr(transform));
        glUniform4fv(UNIFORM_COLOR, 1, glm::value_ptr(model.color));

        // bind vertex array and draw vertices
        glBindVertexArray(model.mesh->vao);
        glDrawArrays(GL_TRIANGLES, 0, model.mesh->vertices_n);
        glBindVertexArray(0);
    }
}

