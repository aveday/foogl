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

static auto sphere_mesh = MeshGen::Sphere(1, 1);
static auto brick_mesh = MeshGen::PdpSquare(100, 2, .3f, 2);

static auto ground_mesh = MeshGen::PdpMesh(2000, 50, .5f, .2);
static auto skybox_mesh = MeshGen::Sphere(100, 2);
static auto cube_mesh   = MeshGen::Box(1, 1, 1);

Model crate{&cube_mesh, "crate.jpg"};
Model ground{&ground_mesh, "grass.png"};
Model skybox{&skybox_mesh, "night.jpg"};

int main() {
    EM::new_entity( Window{"Foogle"}, Clock{});

    int player = EM::new_entity( Controller{}, Camera{},
            Body{{-1, .8, 3}, {1, 1, 1}, {}, {0, 5.6, 0}});

    EM::new_entity(skybox, Body{} );
    EM::new_entity(ground, Body{{}, {1,1,1}, {}, {-M_PI_2,0,0}} );

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
    Body light_body{{0, 10, 5}, vec3(.05)};
    int bulb[] = {
        EM::new_entity( crate, Light{{1, 1, 1}}, light_body ),
        EM::new_entity( crate, Light{{.5, .5, .5}}, light_body ),
        EM::new_entity( crate, Light{{.1, .1, .8}}, light_body ),
    };

    Clock &clock = EM::first_component<Clock>();
    static bool debug = true;

    while (clock.running) {
        vec3 pos{0, 1, 3};
        float angle = fmodf(clock.time, 2*M_PI);
        EM::get_component<Body>(bulb[0]).position.y = .5*sin(clock.time)+1;
        EM::get_component<Body>(bulb[1]).position = glm::rotateY(pos, angle);
        EM::get_component<Body>(bulb[2]).position = glm::rotateY(pos,-angle);

        windowing.run();
        control.run();
        movement.run();
        lighting.run();
        rendering.run();
    }
    return 0;
}

