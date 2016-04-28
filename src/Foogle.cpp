// OpenGL Headers
#include <GL/glew.h>
#include <GL/glu.h>

// Standard Headers
#include <iostream>
#include <list>

// Project Headers
#include "Window.h"
#include "Light.h"
#include "Clock.h"
#include "Camera.h"
#include "Box.h"
#include "Shader.h"
#include "Entity.h"
#include "config.h"
#include "colors.h"
#include "glm.h"

std::list<Entity*> entities;
GLuint lightPositionPtr, lightColorPtr;
mat4 projectionMatrix;

/* MAIN FUNCTION */
int main() {

    Window window("Foogle", screen_width, screen_height, FULLSCREEN, RESIZABLE);

    // create the shader program
    GLuint shader = loadProgram("glsl/flat.vs", "glsl/flat.fs");

    // setup camera
    Camera camera(vec3(0, 5, -10), vec3(0, -1, 2));
    camera.configure(fov, aspect, near, far);

    // setup lighting
    Light light(shader, vec3(0,5,10), white);

    // setup clock
    Clock clock;

    /*       SHADER  SIZE                  POSITION                 COLOR */
    Box box (shader, vec3(5.0f,0.2f,5.0f), vec3(),                  darkRed);
    Box wall(shader, vec3(0.2f,5.0f,5.0f), vec3(2.6f, 2.6f, 0),     darkBlue);

    entities.push_front( &box );
    entities.push_front( &wall);

    /* MAIN EVENT LOOP*/
    while(window.stillOpen())
    {
        float dt = clock.tick();
        window.control(camera);
        window.render(camera, entities);
    }

    window.close(); 
    return 0;
}

