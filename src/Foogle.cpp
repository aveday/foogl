#define EM_MAX_ENTS 10000
#include "EntityManager.h"
#include "AssetLoader.h"
#include "components.h"
#include "core_systems.h"
#include "colors.h"
#include "glm.h"

#include "MeshGen.h"
#include "TexGen.h"

WindowSystem    windowing;
ControlSystem   control;
LightSystem     lighting;
RenderSystem    rendering;
MovementSystem  movement;

// define materials
static MaterialDef dark_check = {"pattern_35/diffus.tga", "pattern_35/normal.tga"};

// generate meshes
static auto ground_mesh = MeshGen::Box(20, 1, 20);
static auto cube_mesh   = MeshGen::Box(1, 1, 1);

Model crate{&cube_mesh, &dark_check};
Model ground{&ground_mesh, &dark_check};

int main() {
    EM::new_entity( Window{"Foogle"}, Clock{});

    int player = EM::new_entity( Controller{}, Camera{},
            Body{{-1, .8, 3}, {1, 1, 1}, {}, {0, 5.6, 0}});

    EM::new_entity(ground, Body{{0,-1,0}, {1,1,1}, {}, {0,0,0}} );

    auto testbox = EM::new_entity( crate, Body{{0, 1, 0}, {1, 1, 1}} );
    // create walls
    EM::new_entity( crate, Body{{3, 0, 0}, {.1, 4, 6}} );
    EM::new_entity( crate, Body{{0, 0, -3}, { 6, 4,.1}} );

    // create ring of n_blocks
    int n_blocks = 6, blocks[n_blocks];
    float block_radius = 2, div = 2 * (float)M_PI / n_blocks;
    Body body{{0, .2, block_radius}, vec3(.5)};
    for(int i = 0; i < n_blocks; ++i) {
        body.position = glm::rotateY(body.position, div);
        body.rotation.y += div;
        blocks[i] = EM::new_entity(crate, body);
    }

    // create lights
    Body light_body{{0, 1, 2}, vec3(.05)};
    int bulb[] = {
        EM::new_entity( crate, Light{{2, 2, 2}}, light_body ),
    };

    Clock &clock = EM::first_component<Clock>();

    while (clock.running) {
        float angle = fmodf(clock.time/2, 2*M_PI);
        EM::get_component<Body>(testbox).rotation = vec3(0, angle, 0);

        windowing.run();
        control.run();
        movement.run();
        lighting.run();
        rendering.run();
    }
    return 0;
}

