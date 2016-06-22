#pragma once
#include <cstdint>
#include "assets.h"
#include "glm.h"

namespace MeshGen {
    MeshDef PdpMesh(int vertices, float size, float amplitude,
                    float period);

    MeshDef Box(float w, float h, float d);

    MeshDef Sphere(float radius, int n);

}
