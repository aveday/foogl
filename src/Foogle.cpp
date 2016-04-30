#include <list>

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

    Mesh cube = Mesh::Cube(shader);
    Mesh invisible(shader);

    // create and populate the entity list
    std::list<Entity*> entities;

    /*                            MODEL POSITION        SCALE         COLOR */
    entities.push_back(new Entity(cube, vec3(0, 0,  0), vec3(5,0.2,5),white*.5f));
    entities.push_back(new Entity(cube, vec3(2.6,.6,0), vec3(0.2,1,5),blue));
    entities.push_back(new Entity(cube, vec3(0,.6,-2.6),vec3(5,1,0.2),green));

    for(float i=-2; i<=2; i+=1)
        entities.push_front(new Entity(cube, vec3(i, 0.5f, -2), vec3(.1,1,.1), darkRed));

    // setup lighting
    Light bulb(cube, vec3(0, 0.5f, -2.4f), white);

    Camera camera(invisible, vec3(0, 0.5f, 0));

    entities.push_back(&camera);
    entities.push_back(&bulb);

    /* MAIN EVENT LOOP*/
    while(window.running())
    {
        float dt = clock.tick();
        float t = clock.time;
        
        bulb.warp( vec3(cos(t), 0.5f, -2.4f) );

        window.control(camera);
        update(entities, dt, shader);
        window.render(camera, entities);
    }
    return 0;
}

