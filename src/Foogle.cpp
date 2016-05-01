#include <list>
#include <math.h>

#include "Window.h"
#include "Light.h"
#include "Clock.h"
#include "Camera.h"
#include "Shader.h"
#include "Entity.h"
#include "config.h"
#include "colors.h"
#include "glm.h"

/* UPDATE FUNCTION */
void update(std::list<Entity*> entities, float dt, GLuint shader)
{
    // simulate world
    for(auto entity : entities)
        entity->update(dt);

    // update shader with new uniforms for lights/camera etc.
    for(auto entity : entities)
        entity->updateShader(shader);
}

/* MAIN FUNCTION */
int main() {

    Window window("Foogle", screen_width, screen_height, FULLSCREEN, RESIZABLE);

    // create the shader program
    GLuint shader = loadProgram("glsl/specular.vs", "glsl/specular.fs");

    // setup clock
    Clock clock;

    // load meshes
    Mesh cube = Mesh::Cube(shader);
    Mesh invisible(shader);

    // create and populate the entity list
    std::list<Entity*> entities;

    /*                            MODEL POSITION        SCALE         COLOR */
    entities.push_back(new Entity(cube, vec3(0, 0,  0), vec3(5,0.2,5),vec4(.5f)));
    entities.push_back(new Entity(cube, vec3(2.6,.6,0), vec3(0.2,1,5),blue));
    entities.push_back(new Entity(cube, vec3(0,.6,-2.6),vec3(5,1,0.2),green));

    int n = 16;
    for(int i=0; i<n; i++)
    {
        float degrees = 360.0f * i / n;
        float radians = 6.28f * i / n;
        float radius = 2;
        vec3 pos(cos(radians) * radius, 0.2f, sin(radians) * radius);
        entities.push_back(new Entity(cube, pos, vec3(.3,.2,.3), darkRed));
        entities.back()->rotate(0, -degrees, 0);
    }

    // setup lighting
    Light bulb(cube, vec3(0, 0.5f, -2.4f), white);

    Camera camera(invisible, vec3(0, 0.5f, 0));

    entities.push_back(&camera);
    entities.push_back(&bulb);

    /* MAIN EVENT LOOP*/
    while(window.running())
    {
        float dt = clock.tick();
        
        float radius = 2;
        float radians = fmodf(clock.time, (2*M_PI));
        vec3 pos(cos(radians) * radius, 0.5f, sin(radians) * radius);
        bulb.warp(pos);

        window.control(camera);
        update(entities, dt, shader);
        window.render(camera, entities);
    }
    return 0;
}

