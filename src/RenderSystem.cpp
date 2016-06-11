#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "EntityManager.h"
#include "RenderSystem.h"
#include "components.h"

RenderSystem::RenderSystem()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
}

void RenderSystem::run(int viewer)
{
    if (!EM::has_components<Camera>(viewer)) {
        std::cerr << "RenderSystem viewer needs Camera component\n";
        exit(EXIT_FAILURE);
    }
    Camera &camera = EM::get_component<Camera>(viewer);
    Body &body = EM::get_component<Body>(viewer);

    // update the camera position uniform
    glUniform3fv(UNIFORM_VIEW_POSITION, 1, glm::value_ptr(body.position));

    // calculate and set the view matrix
    mat4 view_matrix = camera.projection * glm::inverse(body.transform);
    glUniformMatrix4fv(UNIFORM_VIEW_MATRIX, 1, 0, glm::value_ptr(view_matrix));

    for(int e = 0; e < EM::end(); e++) {
        if (!EM::has_components<Model, Body>(e))
            continue;

        Model &model = EM::get_component<Model>(e);
        Body &body = EM::get_component<Body>(e);

        if (model.mesh->vertices_n == 0)
            continue;

        // update uniform shader inputs
        glUniformMatrix4fv(UNIFORM_MODEL,1,0,glm::value_ptr(body.transform));
        glUniform4fv(UNIFORM_COLOR, 1, glm::value_ptr(model.color));

        // bind vertex array and draw vertices
        glBindVertexArray(model.mesh->vao);
        glDrawArrays(GL_TRIANGLES, 0, model.mesh->vertices_n);
        glBindVertexArray(0);
    }
}

