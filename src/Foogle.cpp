// OpenGL Headers
#include <GL/glew.h>
#include <GL/glu.h>

// GLM Headers
#include <glm/glm.hpp>

// Standard Headers
#include <iostream>
#include <list>

// Project Headers
#include "Window.h"
#include "Clock.h"
#include "Box.h"
#include "Shader.h"
#include "Entity.h"
#include "config.h"

std::list<Entity*> entities;
GLuint projectionMatrixPtr, lightPositionPtr, lightColorPtr;
glm::mat4 projectionMatrix;

void updateProjection()
{
    float aspect = (float)screen_width / screen_height;
    projectionMatrix = glm::perspective<GLfloat>(fov, aspect, near, far);
    glUniformMatrix4fv(projectionMatrixPtr, 1, GL_FALSE,
            glm::value_ptr(projectionMatrix));
}

/* RENDER FUNCTION */
void render(Window &window, glm::mat4 &viewMatrix) {

    // manage window resizes
    if(window.hasResized())
        updateProjection();

    // clear screen
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw entities
    for(auto it = entities.begin(); it != entities.end(); it++)
        (*it)->draw(viewMatrix);

    // swap buffers
    window.display();
}

/* MAIN FUNCTION */
int main() {
    
    Window window("Foogle", screen_width, screen_height, FULLSCREEN, RESIZABLE);

    // create the shader program
    GLuint shaderProgram = loadProgram("glsl/flat.vs", "glsl/flat.fs");
    glUseProgram(shaderProgram);

    // setup projection
    projectionMatrixPtr = glGetUniformLocation(shaderProgram, "projection");
    updateProjection();

    // setup lighting
    lightPositionPtr = glGetUniformLocation(shaderProgram, "lightPosition");
    lightColorPtr = glGetUniformLocation(shaderProgram, "lightColor");
    glm::vec3 lightPosition(-3, 4, 0);
    glm::vec3 lightColor(1, 1, 1);
    glUniform3f(lightPositionPtr, lightPosition.x, lightPosition.y, lightPosition.z);
    glUniform3f(lightColorPtr, lightColor.x, lightColor.y, lightColor.z);
    
    // setup clock
    Clock clock;

    // create a large flat box
    Box box(
            shaderProgram,
            glm::vec3(5, 0.2f, 5), // size
            glm::vec3(0, 0, 0),    // position
            glm::vec4(1, 0, 0, 1)  // color
    );

    entities.push_front( &box );

    glm::mat4 viewMatrix = glm::lookAt(
            glm::vec3(0, 5, -10),
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0));

    render(window, viewMatrix);

    /* MAIN EVENT LOOP*/
    while(window.stillOpen())
    {
        // manage time
        float dt = clock.tick();

        // rotate box
        box.rotate(dt, 0, 0);

        // render scene
        render(window, viewMatrix);
    }

    window.close(); 
    return 0;
}

