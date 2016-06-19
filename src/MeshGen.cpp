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

MeshDef MeshGen::Box(float w, float h, float d)
{
    w /= 2, h /= 2, d /= 2;
    std::vector<vec3> positions{
        {-w, -h, -d}, {-w, -h,  d}, {-w,  h, -d}, {-w,  h,  d},
        { w, -h, -d}, { w, -h,  d}, { w,  h, -d}, { w,  h,  d}};
    static std::vector<int> indices{
        0,1,2, 1,3,2,  0,2,6, 0,6,4,  2,3,7, 2,7,6,
        4,6,7, 4,7,5,  1,7,3, 1,5,7,  0,4,1, 1,4,5};
    static std::vector<vec3> normals{
        {-1, 0, 0}, {-1, 0, 0}, { 0, 0,-1}, { 0, 0,-1},
        { 0, 1, 0}, { 0, 1, 0}, { 1, 0, 0}, { 1, 0, 0},
        { 0, 0, 1}, { 0, 0, 1}, { 0,-1, 0}, { 0,-1, 0}};

    return MeshDef{positions, indices, normals};
}
