#define EM_MAX_ENTS 10000
#include "EntityManager.h"
#include "AssetLoader.h"
#include "components.h"
#include "core_systems.h"
#include "colors.h"
#include "glm.h"
#include "cube.h"

WindowSystem    windowing;
ControlSystem   control;
LightSystem     lighting;
RenderSystem    rendering;
MovementSystem  movement;

int main() {
    EM::new_entity( Window{"Foogle"}, Clock{});
    EM::new_entity( Controller{}, Camera{},
            Body{{-3, .5, -3}, {1, 1, 1}, {}, {0, -2.4, 0}});

    // create walls
    EM::new_entity( Model{CUBE, white}, Body{{0,-1, 0}, { 6, 2, 6}} );
    EM::new_entity( Model{CUBE, blue},  Body{{3, 2, 0}, {.1, 4, 6}} );
    EM::new_entity( Model{CUBE, green}, Body{{0, 2, 3}, { 6, 4,.1}} );

    // create ring of n_blocks
    int n_blocks = 16, blocks[n_blocks];
    float radius = 2, div = 2 * (float)M_PI / n_blocks;
    Body body{{0, .2, radius}, {.3, .6, .1}};
    for(int i = 0; i < n_blocks; ++i) {
        body.position = glm::rotateY(body.position, div);
        body.rotation.y += div;
        blocks[i] = EM::new_entity( Model{CUBE, {.1,0,.1,1}}, body);
    }

    // create lights
    Body light_body{vec3(), vec3(.02)};
    int bulb[] = {
        EM::new_entity( Model{CUBE, white}, Light{{.1, .1, .1}}, light_body),
        EM::new_entity( Model{CUBE, red},   Light{{.8, .1, .1}}, light_body),
        EM::new_entity( Model{CUBE, blue},  Light{{.1, .1, .8}}, light_body),
    };

    Clock &clock = EM::first_component<Clock>();

    while (clock.running) {
        vec3 pos{0, .5, 1};
        float angle = fmodf(clock.time, 2*M_PI);
        EM::get_component<Body>(bulb[0]).position.y = .5*sin(4*clock.time)+1;
        EM::get_component<Body>(bulb[1]).position = glm::rotateY(pos, angle);
        EM::get_component<Body>(bulb[2]).position = glm::rotateY(pos,-angle);

        for(int i = 0; i < n_blocks; ++i) {
            Body &b = EM::get_component<Body>(blocks[i]);
            b.position = glm::rotateY(b.position, clock.dt/20);
            b.position.y = 1 + 0.2f * sin(8*M_PI*i/n_blocks + clock.time);
            b.rotation.y = fmodf(clock.time/4 + i, 2*M_PI);
        }

        windowing.run();
        control.run();
        movement.run();
        lighting.run();
        rendering.run();
    }
    return 0;
}

