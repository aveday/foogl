#pragma once
#include "assets.h"
#include "glm.h"

namespace MeshGen {
    MeshDef PdpDisk(int corners, float amplitude, float period, float radius);
    MeshDef Box(float w, float h, float d);
}
