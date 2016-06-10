#include <cstdio>
#include <list>
#include <math.h>

#define EM_MAX_ENTS 10000
#include "EntityManager.h"

#include "AssetLoader.h"
#include "cube.h"

#include "Component.h"
#include "WindowSystem.h"
#include "LightSystem.h"

#include "Camera.h"
#include "Entity.h"
#include "config.h"
#include "colors.h"
#include "glm.h"

/* MAIN FUNCTION */
int main() {

    //Window window("Foogle", screen_width, screen_height, FULLSCREEN, RESIZABLE);
    auto game = EM::new_entity(
            WindowC{"Foogle", screen_width, screen_height},
            ClockC{1.0/60});

    WindowC &root_window = EM::get_component<WindowC>(game);
    WindowSystem windowing(root_window);

    GLuint specular = AL::LoadProgram("glsl/specular.vs", "glsl/specular.fs");
    LightSystem lighting(specular);

    // load meshes
    Mesh invisible;
    Mesh &cube = AL::LoadMesh(cube_def);

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
    auto bulb1 = EM::new_entity( LightC{{.1, .1, .1}, { 0, .5, -2.4f}} );
    auto bulb2 = EM::new_entity( LightC{{.4, .1, .1}, { 2, .5, -2.4f}} );
    auto bulb3 = EM::new_entity( LightC{{.1, .4, .1}, {-2, .5, -2.4f}} );

    Camera camera(invisible, vec3(0, 0.5f, 0));

    entities.push_back(&camera);

    /* MAIN EVENT LOOP*/
    while (EM::has_components<WindowC>(game)) {
        windowing.run();

        ClockC &clock = EM::get_component<ClockC>(game);
        
        float radius = 2;
        float radians = fmodf(clock.time, (2*M_PI));

        vec3 pos1(0, .5, 0);
        vec3 pos2(cos(radians) * radius, 0.5f, sin(radians) * radius);
        vec3 pos3(-cos(radians) * radius, 0.5f, sin(radians) * radius);

        EM::get_component<LightC>(bulb1).position = pos1;
        EM::get_component<LightC>(bulb2).position = pos2;
        EM::get_component<LightC>(bulb3).position = pos3;

        //window.control(camera);

        // simulate world
        for(auto entity : entities)
            entity->update(clock.dt);

        for(auto entity : entities)
            entity->updateShader(specular); //just for camera at this point

        lighting.run();

        // draw entities
        glUseProgram(specular);
        for(auto entity : entities)
            entity->draw();

        glfwSwapBuffers(root_window.gl_window);
    }
    return 0;
}

