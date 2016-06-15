#include "EntityManager.h"
#include "MovementSystem.h"
#include "components.h"
#include "glm.h"

void MovementSystem::run()
{
    float dt = EM::first_component<Clock>().dt;
    for(int e = 0; e < EM::end(); e++) {
        if (!EM::has_components<Body>(e))
            continue;

        Body &body = EM::get_component<Body>(e);
        body.position += dt * glm::rotateY(body.velocity, body.rotation.y);
        body.transform = get_transform(body.position,body.scale,body.rotation);
    }
}
