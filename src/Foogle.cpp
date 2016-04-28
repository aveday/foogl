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
    Light light(shader, vec3(3,7,-7), white);

    // setup clock
    Clock clock;

    /*       SHADER  SIZE                  POSITION                COLOR */
    Box floor(shader,vec3(5.0f,0.2f,5.0f), vec3(0,       0,    0), darkRed);
    Box wall(shader, vec3(0.2f,1.0f,5.0f), vec3(2.6f, 0.6f,    0), darkBlue);
    Box back(shader, vec3(5.0f,1.0f,0.2f), vec3(0,    0.6f, 2.6f), darkGreen);

    floor.add_child(new Box(shader, vec3(0.5f,1.0f,0.5f), vec3(0, 1, 0), white));

    // setup player and player camera
    Box player(shader, vec3(0.1f,0.1f,0.1f), vec3(2,0.5f,-2), Color(0,0,0,0));

    Camera camera(vec3(0, 0, 0.5f));
    player.add_child(&camera);

    entities.push_front( &player);
    entities.push_front( &floor);
    entities.push_front( &wall);
    entities.push_front( &back);

    /* MAIN EVENT LOOP*/
    while(window.running())
    {
        float dt = clock.tick();
        //player.rotate(0,0,100*dt);
        window.control(player);
        window.update(entities, dt);
        window.render(camera, entities);
    }
    return 0;
}

