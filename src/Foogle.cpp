#include <cstdio>
#include <list>
#include <math.h>

#define EM_MAX_ENTS 10000
#include "EntityManager.h"

#include "Component.h"
#include "WindowSystem.h"

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
    WindowSystem windowing;

    //Window window("Foogle", screen_width, screen_height, FULLSCREEN, RESIZABLE);
    auto game = EM::new_entity(
            WindowC{"Foogle", screen_width, screen_height},
            ClockC{1.0/60});

    windowing.run();
    GLFWwindow *gl_window = EM::get_component<WindowC>(game).gl_window;

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
    Light bulb1(shader, cube, vec3(0, 0.5f, -2.4f), white*0.1f);
    Light bulb2(shader, cube, vec3(0, 0.5f, -2.4f), vec4(.4, .1, .1, 1));
    Light bulb3(shader, cube, vec3(0, 0.5f, -2.4f), vec4(.1, .4, .1, 1));

    Camera camera(invisible, vec3(0, 0.5f, 0));

    entities.push_back(&camera);

    entities.push_back(&bulb1);
    entities.push_back(&bulb2);
    entities.push_back(&bulb3);

    /* MAIN EVENT LOOP*/
    while (EM::has_components<WindowC>(game)) {
        float dt = clock.tick();
        
        float radius = 2;
        float radians = fmodf(clock.time, (2*M_PI));

        vec3 pos1(0, .5, 0);
        vec3 pos2(cos(radians) * radius, 0.5f, sin(radians) * radius);
        vec3 pos3(-cos(radians) * radius, 0.5f, sin(radians) * radius);

        bulb1.warp(pos1);
        bulb2.warp(pos2);
        bulb3.warp(pos3);

        //window.control(camera);
        update(entities, dt, shader);

        //window.render(camera, entities) {
            // clear screen
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // draw entities
            for(auto entity : entities)
                entity->draw();

            glfwSwapBuffers(gl_window);
        //}
    }
    return 0;
}

