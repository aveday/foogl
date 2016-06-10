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
#include "RenderSystem.h"

#include "Camera.h"
#include "Entity.h"
#include "config.h"
#include "colors.h"
#include "glm.h"

int main() {

    auto game = EM::new_entity(
            WindowC{"Foogle", screen_width, screen_height},
            ClockC{1.0/60});

    WindowC &root_window = EM::get_component<WindowC>(game);
    WindowSystem windowing(root_window);

    GLuint specular = AL::LoadProgram("glsl/specular.vs", "glsl/specular.fs");
    LightSystem lighting(specular); //FIXME for multiple shader support

    RenderSystem rendering;

    // load meshes
    Mesh invisible;
    Mesh &cube = AL::LoadMesh(cube_def);

    // create walls
    EM::new_entity(ModelC{&cube, white, translate(0,0,0)    * scale(5,.2,5)});
    EM::new_entity(ModelC{&cube, blue,  translate(2.6,.6,0) * scale(.2,1,5)});
    EM::new_entity(ModelC{&cube, green, translate(0,.6,-2.6)* scale(5,1,.2)});

    // create ring of red blocks
    int blocks = 16;
    float radius = 2;
    mat4 transform = translate(0, .2, radius) * scale(.3, .2, .3);
    for(int i = 0; i < blocks; ++i) {
        transform = rotate(0, 2*M_PI/blocks, 0) * transform;
        EM::new_entity( ModelC{&cube, darkRed, transform} );
    }

    // create lights
    int bulb[] = {
        EM::new_entity( LightC{{.1, .1, .1}, { 0, .5, -2.4f}} ),
        EM::new_entity( LightC{{.4, .1, .1}, { 2, .5, -2.4f}} ),
        EM::new_entity( LightC{{.1, .4, .1}, {-2, .5, -2.4f}} )};

    // setup camera
    std::list<Entity*> entities;
    Camera camera(invisible, vec3(0, 0.5f, 0));
    entities.push_back(&camera);

    while (EM::has_components<WindowC>(game)) {
        windowing.run();

        ClockC &clock = EM::get_component<ClockC>(game);
        
        float radius = 2;
        float radians = fmodf(clock.time, (2*M_PI));

        vec3 pos1(0, .5, 0);
        vec3 pos2(cos(radians) * radius, 0.5f, sin(radians) * radius);
        vec3 pos3(-cos(radians) * radius, 0.5f, sin(radians) * radius);

        EM::get_component<LightC>(bulb[0]).position = pos1;
        EM::get_component<LightC>(bulb[1]).position = pos2;
        EM::get_component<LightC>(bulb[2]).position = pos3;

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

        rendering.run();

        glfwSwapBuffers(root_window.gl_window);
    }
    return 0;
}

