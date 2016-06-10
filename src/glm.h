#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

inline mat4 scale(float x, float y, float z)
{
    return glm::scale(vec3(x, y, z));
}

inline mat4 translate(float x, float y, float z)
{
    return glm::translate(vec3(x, y, z));
}

inline mat4 rotate(float x, float y, float z)
{
    mat4 transform;
    if (y) transform = glm::rotate(transform, y, vec3(0,1,0));
    if (x) transform = glm::rotate(transform, x, vec3(1,0,0));
    if (z) transform = glm::rotate(transform, z, vec3(0,0,1));
    return transform;
}

