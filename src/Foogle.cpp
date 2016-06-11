#include <iostream>
#include <math.h>

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
            Body{.position={0, .5, 0}});

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
    EM::new_entity( Model{&cube, white},
                    Body{.position={0.0, 0.0, 0.0}, .scale={5.0, 0.2, 5.0}});
    EM::new_entity( Model{&cube, blue},
                    Body{.position={2.6, 0.6, 0.0}, .scale={0.2, 1.0, 5.0}});
    EM::new_entity( Model{&cube, green},
                    Body{.position={0.0, 0.6,-2.6}, .scale={5.0, 1.0, 0.2}});

    // create ring of red blocks
    int blocks = 20;
    float radius = 2;
    Body body{.position={0, .2, radius}, .scale={.3, .2, .3}};
    for(int i = 0; i < blocks; ++i) {
        body.position = glm::rotateY(body.position, 2*(float)M_PI/blocks);
        body.rotation.y += 2*(float)M_PI/blocks;
        EM::new_entity( Model{&cube, darkRed}, body);
    }

    // create lights
    int bulb[] = {
        EM::new_entity( Light{{.1, .1, .1}}, Body{.position={0, .5, 0}} ),
        EM::new_entity( Light{{.4, .1, .1}}, Body{} ),
        EM::new_entity( Light{{.1, .1, .4}}, Body{} )};

    while (root_window.gl_window) {
        vec3 pos{0, .5, 2};
        float angle = fmodf(clock.time, (2*M_PI));
        EM::get_component<Body>(bulb[0]).position.y = .5*sin(4*clock.time)+1;
        EM::get_component<Body>(bulb[1]).position = glm::rotateY(pos, angle);
        EM::get_component<Body>(bulb[2]).position = glm::rotateY(pos,-angle);

        lighting.run();
        rendering.run(player);
        windowing.run(root_window, clock);
        control.run(player);
        movement.run(clock.dt);
    }
    return 0;
}

