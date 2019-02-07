#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "RenderSystem.h"
#include "EntityManager.h"
#include "AssetLoader.h"
#include "components.h"

void RenderSystem::run()
{
    static bool init = false;
    if (!init) {
        init = true;
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_CULL_FACE);
        glUseProgram(AL::LoadProgram("glsl/specular.vs", "glsl/specular.fs"));//FIXME
    }

    int viewer = EM::first_entity<Camera, Body>();
    Camera &camera = EM::get_component<Camera>(viewer);
    Body &body = EM::get_component<Body>(viewer);

    //int width, height;
    //glfwGetWindowSize(window.gl_window, &width, &height);
    //camera.aspect = (float)width/height;

    // update the camera position uniform
    glUniform3fv(UNIFORM_VIEW_POSITION, 1, glm::value_ptr(body.position));

    // calculate and set the view matrix
    mat4 view_matrix = camera.projection * glm::inverse(body.transform);
    glUniformMatrix4fv(UNIFORM_VIEW_MATRIX, 1, 0, glm::value_ptr(view_matrix));

    for(int e = 0; e < EM::end(); e++) {
        if (!EM::has_components<Model, Material, Body>(e))
            continue;

        Model &model = EM::get_component<Model>(e);
        Material &material = EM::get_component<Material>(e);
        Body &body = EM::get_component<Body>(e);

        //FIXME - shouldn't be checked like this, shouldn't be done so often
        if (!model.mesh.vertices_n)
            model.mesh = AL::LoadMesh(*model.mesh_def);

        // update uniform shader inputs
        glUniformMatrix4fv(UNIFORM_MODEL, 1, 0, glm::value_ptr(body.transform));
        glUniform4fv(UNIFORM_MATERIAL_COLOR, 1, glm::value_ptr(material.color));

        // bind vertex array and draw vertices
        glBindVertexArray(model.mesh.vao);
        glDrawArrays(GL_TRIANGLES, 0, model.mesh.vertices_n);
        glBindVertexArray(0);
    }
}

