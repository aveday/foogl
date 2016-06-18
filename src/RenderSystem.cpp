#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "EntityManager.h"
#include "AssetLoader.h"
#include "RenderSystem.h"
#include "components.h"

void RenderSystem::run()
{
    static bool init = false;
    if (!init) {
        init = true;
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_CULL_FACE);
        glEnable(GL_TEXTURE_2D);
        glUseProgram(AL::LoadProgram("glsl/specular.vs", "glsl/specular.fs"));
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
        if (!EM::has_components<Model, Body>(e))
            continue;

        Model &model = EM::get_component<Model>(e);
        Body &body = EM::get_component<Body>(e);

        if (!model.mesh.vertices_n)
            model.mesh = AL::LoadMesh(*model.def);
        if (!model.texture)
            model.texture = AL::LoadTexture(model.texture_file);

        // bind the texture and set uniform
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, model.texture);
        glUniform1i(UNIFORM_TEXTURE, 0);

        // update uniform shader inputs
        glUniformMatrix4fv(UNIFORM_MODEL,1,0,glm::value_ptr(body.transform));

        // bind vertex array and draw vertices
        glBindVertexArray(model.mesh.vao);
        glDrawArrays(GL_TRIANGLES, 0, model.mesh.vertices_n);
        glBindVertexArray(0);
    }
}

