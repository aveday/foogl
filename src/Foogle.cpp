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

#include "config.h"
#include "colors.h"
#include "glm.h"

int main() {

    auto game = EM::new_entity( Window{"Foogle"}, Clock{1.0/60});
    auto player = EM::new_entity( Camera{}, Transform(translate(0, .5, 0)));

    Camera &cam = EM::get_component<Camera>(player);
    Window &root_window = EM::get_component<Window>(game);

    WindowSystem windowing(root_window);
    LightSystem lighting;
    RenderSystem rendering;

    GLuint specular = AL::LoadProgram("glsl/specular.vs", "glsl/specular.fs");
    glUseProgram(specular);

    // load meshes
    Mesh cube = AL::LoadMesh(cube_def);

    // create walls
    EM::new_entity( Model{&cube, white},
                    Transform{translate(0.0, 0.0, 0.0)*scale(5.0, 0.2, 5.0)});
    EM::new_entity( Model{&cube, blue},
                    Transform{translate(2.6, 0.6, 0.0)*scale(0.2, 1.0, 5.0)});
    EM::new_entity( Model{&cube, green},
                    Transform{translate(0.0, 0.6,-2.6)*scale(5.0, 1.0, 0.2)});

    // create ring of red blocks
    int blocks = 20;
    float radius = 2;
    mat4 transform = translate(0, .2, radius) * scale(.3, .2, .3);
    for(int i = 0; i < blocks; ++i) {
        transform = rotate(0, 2*M_PI/blocks, 0) * transform;
        EM::new_entity( Model{&cube, darkRed}, Transform{transform} );
    }

    // create lights
    int bulb[] = {
        EM::new_entity( Light{{.1, .1, .1}, { 0, .5, -2.4f}} ),
        EM::new_entity( Light{{.4, .1, .1}, { 2, .5, -2.4f}} ),
        EM::new_entity( Light{{.1, .4, .1}, {-2, .5, -2.4f}} )};

    while (EM::has_components<Window>(game)) {
        windowing.run();

        Clock &clock = EM::get_component<Clock>(game);
        
        float radius = 2;
        float radians = fmodf(clock.time, (2*M_PI));

        vec3 pos1(0, .5, 0);
        vec3 pos2(cos(radians) * radius, 0.5f, sin(radians) * radius);
        vec3 pos3(-cos(radians) * radius, 0.5f, sin(radians) * radius);

        EM::get_component<Light>(bulb[0]).position = pos1;
        EM::get_component<Light>(bulb[1]).position = pos2;
        EM::get_component<Light>(bulb[2]).position = pos3;

        //window.control(camera);
        lighting.run();
        rendering.run(player);

        glfwSwapBuffers(root_window.gl_window);
    }
    return 0;
}

