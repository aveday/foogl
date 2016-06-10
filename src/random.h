#pragma once

inline float random_f()
{
    return (float)rand() / RAND_MAX;
}

inline float random_f(float max)
{
    return random_f() * max;
}

inline float random_f(float min, float max)
{
    return random_f(max - min) + min;
}

inline int random_i(int min, int max)
{
    return random_f() * (min + max) - min;
}

