#pragma once
#include <cstdio>

inline int random_i(int max)
{
    return rand() % max;
}

inline float random_f()
{
    return float(rand()) / RAND_MAX;
}

inline float random_f(float max)
{
    return random_f() * max;
}

inline float random_f(float min, float max)
{
    return random_f(max - min) + min;
}
