#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include "AssetLoader.h"
#include "TexGen.h"
#include "MeshGen.h"
#include "RenderSystem.h"

GLuint TexGen::MeshNormals(MeshDef &def, ivec2 resolution)
{
    // Generate framebuffer names
    GLuint framebuffer = 0;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, resolution.x, resolution.y);

    // The texture we're going to render to
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, resolution.x, resolution.y,
                                0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    /* The depth buffer
    GLuint depthrenderbuffer;
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, depthrenderbuffer);
    */

    // Set "texture" as our colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the resolution of DrawBuffers

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "framebuffer not okay\n";
        exit(EXIT_FAILURE);
    }


    // clear with white
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(AL::LoadProgram("glsl/normals.vs", "glsl/normals.fs"));

    // set the camera position uniform to the origin
    mat4 pos = glm::translate( vec3{0, 0,  0} );

    // calculate and set an orthogonal view matrix
    mat4 view_matrix = glm::ortho(-1.f, 1.f, -1.f, 1.f) * glm::inverse(pos);
    glUniformMatrix4fv(UNIFORM_VIEW_MATRIX, 1, 0, glm::value_ptr(view_matrix));

    // render to texture
    Mesh mesh = AL::LoadMesh(def);
    glBindVertexArray(mesh.vao);
    glDrawArrays(GL_TRIANGLES, 0, mesh.vertices_n);

    glBindVertexArray(0);

    // unbind texture and framebuffer
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(AL::LoadProgram("glsl/specular.vs", "glsl/specular.fs"));

    return texture;
}
