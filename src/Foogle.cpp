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

/* UPDATE FUNCTION */
void update(std::list<Entity*> entities, float dt, GLuint shader)
{
    // simulate world
    for(auto it = entities.begin(); it != entities.end(); it++)
        (*it)->update(dt);

    // update shader with new uniforms for lights/camera etc.
    for(auto it = entities.begin(); it != entities.end(); it++)
        (*it)->updateShader(shader);
}

/* MAIN FUNCTION */
int main() {

    Window window("Foogle", screen_width, screen_height, FULLSCREEN, RESIZABLE);

    // create the shader program
    GLuint shader = loadProgram("glsl/specular.vs", "glsl/specular.fs");

    // setup clock
    Clock clock;

    // create and populate the entity list
    std::list<Entity*> entities;

    /*       SHADER  SIZE                  POSITION                COLOR */
    Box floor(shader,vec3(5.0f,0.2f,5.0f), vec3(0,       0,    0), darkRed);
    Box wall(shader, vec3(0.2f,1.0f,5.0f), vec3(2.6f, 0.6f,    0), darkBlue);
    Box back(shader, vec3(5.0f,1.0f,0.2f), vec3(0,    0.6f,-2.6f), darkGreen);

    for(float i=-2; i<=2; i+=1)
        entities.push_front(new Box(shader, vec3(0.1f,1.0f,0.1f), vec3(i, 0.1f, -2), white));

    // setup player and player camera
    Box player(shader, vec3(0.1f,0.1f,0.1f), vec3(0,0.5f,-1), Color(0,1,1,1));

    // setup lighting
    Light light(shader, vec3(0, 0.5f, -2.4f), white);

    Camera camera(vec3(0, 0.5f, 0));

    entities.push_front( &player);
    entities.push_front( &floor);
    entities.push_front( &wall);
    entities.push_front( &back);
    entities.push_front( &camera);
    entities.push_front( &light);

    /* MAIN EVENT LOOP*/
    while(window.running())
    {
        float dt = clock.tick();
        float t = clock.time;
        vec3 p(cos(t), 0.5f, -2.4f);

        player.warp(p);
        light.warp(p);

        window.control(camera);
        update(entities, dt, shader);
        window.render(camera, entities);
    }
    return 0;
}

