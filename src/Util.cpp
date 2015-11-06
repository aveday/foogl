
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

