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
        glEnable(GL_TEXTURE_2D);

		GLuint programID = AL::LoadProgram("glsl/specular.vs", "glsl/specular.fs");//FIXME
        glUseProgram(programID);

		MatrixID = glGetUniformLocation(programID, "MVP");
		ModelMatrixID = glGetUniformLocation(programID, "M");
		ViewMatrixID = glGetUniformLocation(programID, "V");
		ModelView3x3MatrixID = glGetUniformLocation(programID, "MV3x3");
    }

    int viewer = EM::first_entity<Camera, Body>();
    Camera &camera = EM::get_component<Camera>(viewer);
    Body &viewer_body = EM::get_component<Body>(viewer);

    //int width, height;
    //glfwGetWindowSize(window.gl_window, &width, &height);
    //camera.aspect = (float)width/height;

    // update the camera position uniform
    //glUniform3fv(UNIFORM_VIEW_POSITION, 1, glm::value_ptr(viewer_body.position));

    // calculate and set the view matrix
    //mat4 view_matrix = camera.projection * glm::inverse(body.transform);
    //glUniformMatrix4fv(UNIFORM_VIEW_MATRIX, 1, 0, glm::value_ptr(view_matrix));

    for(int e = 0; e < EM::end(); e++) {
        if (!EM::has_components<Model, Body>(e))
            continue;

        Model &model = EM::get_component<Model>(e);
        Body &body = EM::get_component<Body>(e);

        //FIXME - shouldn't be checked like this, shouldn't be done so often
        if (!model.mesh.vertices_n)
            model.mesh = AL::LoadMesh(*model.mesh_def);
        if (!model.material.diffuse)
            model.material = AL::LoadMaterial(*model.material_def);

        // bind the texture maps and set uniforms
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, model.material.diffuse);
        glUniform1i(UNIFORM_DIFFUSEMAP, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, model.material.normal);
        glUniform1i(UNIFORM_NORMALMAP, 1);

		glm::mat4 ProjectionMatrix = camera.projection;
		glm::mat4 ViewMatrix = glm::inverse(viewer_body.transform);
		glm::mat4 ModelMatrix = body.transform;
		glm::mat4 ModelViewMatrix = ViewMatrix * ModelMatrix;
		glm::mat3 ModelView3x3Matrix = glm::mat3(ModelViewMatrix);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        // update uniform shader inputs
        glUniformMatrix4fv(UNIFORM_MODEL,1,0,glm::value_ptr(body.transform));

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
		glUniformMatrix3fv(ModelView3x3MatrixID, 1, GL_FALSE, &ModelView3x3Matrix[0][0]);

        // bind vertex array and draw vertices
        glBindVertexArray(model.mesh.vao);
        glDrawArrays(GL_TRIANGLES, 0, model.mesh.vertices_n);
        glBindVertexArray(0);
    }
}

