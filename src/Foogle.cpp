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
#include "Camera.h"
#include "Box.h"
#include "Shader.h"
#include "Entity.h"
#include "config.h"

std::list<Entity*> entities;
GLuint lightPositionPtr, lightColorPtr;
glm::mat4 projectionMatrix;

/* MAIN FUNCTION */
int main() {

    Window window("Foogle", screen_width, screen_height, FULLSCREEN, RESIZABLE);
    float aspect = (float)screen_width / screen_height;

    // create the shader program
    GLuint shaderProgram = loadProgram("glsl/flat.vs", "glsl/flat.fs");
    glUseProgram(shaderProgram);

    // setup camera
    Camera camera(fov, aspect, near, far);

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
            glm::vec3(5, 0.2f, 5),  // size
            glm::vec3(0, 0, 0),     // position
            glm::vec4(1, 0, 0, 1)); // color

    entities.push_front( &box );

    window.render(camera, entities);

    /* MAIN EVENT LOOP*/
    while(window.stillOpen())
    {
        // manage time
        float dt = clock.tick();

        // rotate box
        box.rotate(dt, 0, 0);

        // render scene
        window.render(camera, entities);
    }

    window.close(); 
    return 0;
}

