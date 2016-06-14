#define EM_MAX_ENTS 10000
#include "EntityManager.h"
#include "AssetLoader.h"

#include "components.h"
#include "LightSystem.h"
#include "RenderSystem.h"
#include "WindowSystem.h"
#include "ControlSystem.h"
#include "MovementSystem.h"

#include "colors.h"
#include "glm.h"
#include "cube.h"

int main() {

    auto game = EM::new_entity( Window{"Foogle"}, Clock{1.0/60});

    auto player = EM::new_entity(
            Controller{}, Camera{},
            Body{{-3, .5, -3}, {1, 1, 1}, {}, {0, -2.4, 0}});

    Window &root_window = EM::get_component<Window>(game);
    Clock &clock = EM::get_component<Clock>(game);

    WindowSystem windowing(root_window);
    ControlSystem control(root_window);
    LightSystem lighting;
    RenderSystem rendering;
    MovementSystem movement;

    glUseProgram( AL::LoadProgram("glsl/specular.vs", "glsl/specular.fs") );

    // load meshes
    Mesh cube = AL::LoadMesh(cube_def);

    // create walls
    EM::new_entity( Model{&cube, white}, Body{{0,-1, 0}, { 6, 2, 6}});
    EM::new_entity( Model{&cube, blue},  Body{{3, 2, 0}, {.1, 4, 6}});
    EM::new_entity( Model{&cube, green}, Body{{0, 2, 3}, { 6, 4,.1}});

    // create ring of n_blocks
    int n_blocks = 16, blocks[n_blocks];
    float radius = 2;
    Body body{{0, .2, radius}, {.3, .6, .1}};
    for(int i = 0; i < n_blocks; ++i) {
        body.position = glm::rotateY(body.position, 2*(float)M_PI/n_blocks);
        body.rotation.y += 2*(float)M_PI/n_blocks;
        blocks[i] = EM::new_entity( Model{&cube, {.1,0,.1,1}}, body);
    }

    // create lights
    Body light_body{vec3(), vec3(.02)};
    int bulb[] = {
        EM::new_entity( Model{&cube, white}, Light{{.1, .1, .1}}, light_body),
        EM::new_entity( Model{&cube, red},   Light{{.8, .1, .1}}, light_body),
        EM::new_entity( Model{&cube, blue},  Light{{.1, .1, .8}}, light_body),
    };

    while (root_window.gl_window) {
        vec3 pos{0, .5, 1};
        float angle = fmodf(clock.time, (2*M_PI));
        EM::get_component<Body>(bulb[0]).position.y = .5*sin(4*clock.time)+1;
        EM::get_component<Body>(bulb[1]).position = glm::rotateY(pos, angle);
        EM::get_component<Body>(bulb[2]).position = glm::rotateY(pos,-angle);

        vec3 ppos = EM::get_component<Body>(player).position;
        for(int i = 0; i < n_blocks; ++i) {
            Body &b = EM::get_component<Body>(blocks[i]);
            b.position = glm::rotateY(b.position, clock.dt*0.05f);
            b.position.y = 1+.3 * sin(8*M_PI*i/n_blocks + clock.time);
            b.rotation.y = fmodf(clock.time/4 + i, 2*M_PI);
            b.rotation.z = 4*atan((ppos.z-b.position.z)/(ppos.x-b.position.x));
        }

        lighting.run();
        rendering.run(player);
        windowing.run(root_window, clock);
        control.run(player);
        movement.run(clock.dt);
    }
    return 0;
}

