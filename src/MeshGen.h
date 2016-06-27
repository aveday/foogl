#pragma once
#include <cstdint>
#include "assets.h"
#include "glm.h"

namespace MeshGen {
    MeshDef PdpMesh( const int vertices = 1000,
                     const float size = 3.0f,
                     const float amplitude = 0.2f,
                     const float period = 4 );

    MeshDef Box(float w, float h, float d);

    MeshDef Sphere(float radius, int n);

}
