#include "EntityManager.h"
#include "MovementSystem.h"
#include "components.h"
#include "glm.h"

void MovementSystem::run(float dt)
{
    for(int e = 0; e < EM::end(); e++) {
        if (!EM::has_components<Velocity>(e))
            continue;

        Velocity &velocity = EM::get_component<Velocity>(e);
        Transform &transform = EM::get_component<Transform>(e);

        vec3 position(transform[3]);

        transform = glm::translate(dt * velocity) * transform;
    }
}
