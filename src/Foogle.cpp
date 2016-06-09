#include <cstdio>
#include <list>
#include <math.h>

#define EM_MAX_ENTS 10000
#include "EntityManager.h"

#include "AssetLoader.h"

#include "Component.h"
#include "WindowSystem.h"

#include "Light.h"
#include "Camera.h"
#include "Entity.h"
#include "config.h"
#include "colors.h"
#include "glm.h"

/* MAIN FUNCTION */
int main() {
    WindowSystem windowing;

    //Window window("Foogle", screen_width, screen_height, FULLSCREEN, RESIZABLE);
    auto game = EM::new_entity(
            WindowC{"Foogle", screen_width, screen_height},
            ClockC{1.0/60});

    //bulb2(specular, cube, vec3( 2, 0.5f, -2.4f), vec3(.4, .1, .1));

    windowing.run();//FIXME remove when ECS is fully integrated
    GLFWwindow *gl_window = EM::get_component<WindowC>(game).gl_window;

    // create the shader program
    GLuint specular = AL::LoadProgram("glsl/specular.vs", "glsl/specular.fs");

    // load meshes
    Mesh cube = Mesh::Cube(specular);
    Mesh invisible(specular);

    // create and populate the entity list
    std::list<Entity*> entities;
    std::list<Light*> lights;

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
    Light bulb1(specular, cube, vec3( 0, 0.5f, -2.4f), vec3(0.1f));
    Light bulb2(specular, cube, vec3( 2, 0.5f, -2.4f), vec3(.4, .1, .1));
    Light bulb3(specular, cube, vec3(-2, 0.5f, -2.4f), vec3(.1, .4, .1));

    Camera camera(invisible, vec3(0, 0.5f, 0));

    entities.push_back(&camera);

    lights.push_back(&bulb1);
    lights.push_back(&bulb2);
    lights.push_back(&bulb3);
    entities.push_back(&bulb1);
    entities.push_back(&bulb2);
    entities.push_back(&bulb3);

    /* MAIN EVENT LOOP*/
    while (EM::has_components<WindowC>(game)) {
        windowing.run();

        ClockC &clock = EM::get_component<ClockC>(game);
        
        float radius = 2;
        float radians = fmodf(clock.time, (2*M_PI));

        vec3 pos1(0, .5, 0);
        vec3 pos2(cos(radians) * radius, 0.5f, sin(radians) * radius);
        vec3 pos3(-cos(radians) * radius, 0.5f, sin(radians) * radius);

        bulb1.warp(pos1);
        bulb2.warp(pos2);
        bulb3.warp(pos3);

        //window.control(camera);

        // simulate world
        for(auto entity : entities)
            entity->update(clock.dt);
        for(auto entity : entities)
            entity->updateShader(specular); //just for camera at this point

        // update shader with new uniforms for lights/camera etc.
        int n = 0;
        for(auto light : lights)
            light->updateLighting(n++);

        // draw entities
        for(auto entity : entities)
            entity->draw();

        glfwSwapBuffers(gl_window);
    }
    return 0;
}

