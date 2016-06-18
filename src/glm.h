#pragma once
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/euler_angles.hpp>


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

inline vec3 get_normal(vec3 v[3])
{
    vec3 j = v[1] - v[0];
    vec3 k = v[2] - v[0];
    return normalize( vec3(
        k.y * j.z - k.z * j.y,
        k.z * j.x - k.x * j.z,
        k.x * j.y - k.y * j.x) );
}

inline mat4 get_transform(vec3 position, vec3 scale, vec3 rotation)
{
    return glm::translate(position)
         * glm::yawPitchRoll(rotation.y, rotation.x, rotation.z)
         * glm::scale(scale);
}

inline void set_transform(mat4 &transform,
        const vec3 &position, const vec3 &scale, const vec3 &rotation)
{
    transform = glm::translate(position)
         * glm::yawPitchRoll(rotation.y, rotation.x, rotation.z)
         * glm::scale(scale);
}

inline void euler_normalise(vec3 &rotation)
{
    rotation.y = glm::mod(rotation.y, 2*(float)M_PI);
    rotation.x = glm::clamp(rotation.x, -(float)M_PI, (float)M_PI);
}
