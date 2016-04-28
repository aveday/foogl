#include <list>

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

/* MAIN FUNCTION */
int main() {

    Window window("Foogle", screen_width, screen_height, FULLSCREEN, RESIZABLE);

    // create the shader program
    GLuint shader = loadProgram("glsl/flat.vs", "glsl/flat.fs");

    // setup lighting
    Light light(shader, vec3(0,7,-7), white);

    // setup clock
    Clock clock;

    Box player(shader, vec3(1,1,1), vec3(0,3,0), green);
    player.rotate(0, 0, 0);

    /*       SHADER  SIZE                  POSITION          COLOR */
    Box box (shader, vec3(5.0f,0.2f,5.0f), vec3(0,-2.6f, 0), darkRed);
    Box wall(shader, vec3(0.2f,5.0f,5.0f), vec3(2.4f, 0, 0), darkBlue);
    Box back(shader, vec3(5.0f,5.0f,0.2f), vec3(0, 0, 2.6f), darkGreen);

    // setup camera
    Camera camera(vec3(0, 0, -10));
    camera.configure(fov, aspect, near, far);

    player.add_child(&camera);

    entities.push_front( &player);
    entities.push_front( &box );
    entities.push_front( &wall);
    entities.push_front( &back);

    /* MAIN EVENT LOOP*/
    while(window.running())
    {
        float dt = clock.tick();
        //player.rotate(0,0,100*dt);
        window.control(player);
        window.render(camera, entities);
    }
    return 0;
}

