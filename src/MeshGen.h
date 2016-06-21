#pragma once
#include <cstdint>
#include "assets.h"
#include "glm.h"

namespace MeshGen {
    MeshDef PdpMesh(int vertices, float size, float amplitude,
                    float period, MeshFlags flags = 0);

    MeshDef Box(float w, float h, float d, MeshFlags flags = SURFACE_NORMALS);

    MeshDef Sphere(float radius, int n, MeshFlags flags = POSITION_NORMALS);

}
