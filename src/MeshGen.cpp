#include "MeshGen.h"
#include "assets.h"

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"
#include "triangulate.h"
#include "poisson.h"

MeshDef MeshGen::PdpDisk( int corners = 1000,
                          float radius = 3.0f,
                          float amplitude = 0.2f,
                          float period = 4 )
{
    std::vector<vec3> positions;

    for (auto p : GeneratePoissonPoints(corners)) {
        float x = radius * (p.x - .5f);
        float z = radius * (p.y - .5f);
        float y = stb_perlin_noise3(x*period, 0, z*period) * amplitude;
        positions.push_back({ x, y, z});
    }

    std::vector<int> indices = triangulate(positions);
    std::vector<vec3> normals = get_normals(positions, indices);

    return MeshDef{positions, indices, normals};
}

