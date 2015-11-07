
#include "Util.h"

float random_f()
{
    return (float)rand() / RAND_MAX;
}

float random_f(float max)
{
    return random_f() * max;
}

float random_f(float min, float max)
{
    return random_f(max - min) + min;
}

int random_i(int min, int max)
{
    return random_f() * (min + max) - min;
}

glm::vec3 getNormal(glm::vec3 v[3])
{
    glm::vec3 j = v[1] - v[0];
    glm::vec3 k = v[2] - v[0];
    return normalize( glm::vec3(
        k.y * j.z - k.z * j.y,
        k.z * j.x - k.x * j.z,
        k.x * j.y - k.y * j.x) );
}
