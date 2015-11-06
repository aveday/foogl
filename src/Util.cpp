
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

glm::vec3 getNormal(glm::vec3 v[3])
{
    glm::vec3 j = v[1] - v[0];
    glm::vec3 k = v[2] - v[0];
    return glm::vec3(
        k.y * j.z - k.z * j.y,
        k.z * j.x - k.x * j.z,
        k.x * j.y - k.y * j.x);
}
/*
public static Vector3 GetNormal(Vector3 v0, Vector3 v1, Vector3 v2)
{
    Vector3 u = v1 - v0;
    Vector3 v = v2 - v0;
    return new Vector3(
        u.Y * v.Z - u.Z * v.Y,
        u.Z * v.X - u.X * v.Z,
        u.X * v.Y - u.Y * v.X);
}
*/ 
