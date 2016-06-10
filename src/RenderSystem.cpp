#include <glm/gtc/type_ptr.hpp>
#include "EntityManager.h"
#include "RenderSystem.h"
#include "Component.h"

void RenderSystem::run()
{
    for(int e = 0; e < EM::end(); e++) {
        if ( !EM::has_components<ModelC>(e) )
            continue;

        ModelC &model = EM::get_component<ModelC>(e);


        if (model.mesh->vertices_n == 0)
            return;

        // update uniform shader inputs
        glUniformMatrix4fv(UNIFORM_MODEL, 1, 0, glm::value_ptr(model.transform));
        glUniform4fv(UNIFORM_COLOR, 1, glm::value_ptr(model.color));

        // bind vertex array and draw vertices
        glBindVertexArray(model.mesh->vao);
        glDrawArrays(GL_TRIANGLES, 0, model.mesh->vertices_n);
        glBindVertexArray(0);
    }
}

