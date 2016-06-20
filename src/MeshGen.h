#pragma once
#include <cstdint>
#include "assets.h"
#include "glm.h"

constexpr uint16_t INVERTED     = 1;
constexpr uint16_t NULL_NORMALS = 2;

namespace MeshGen {
    MeshDef PdpDisk(int corners, float radius, float amplitude, float period);
    MeshDef Box(float w, float h, float d);
    MeshDef Sphere(float radius, int n, uint16_t flags);

}
