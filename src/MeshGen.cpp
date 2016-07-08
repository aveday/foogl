#include "MeshGen.h"
#include "assets.h"

#define PAR_SHAPES_IMPLEMENTATION
#include "par_shapes.h"
#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"
#include "triangulate.h"
#include "poisson.h"

MeshDef MeshGen::PdpSquare( const int vertices,
                          const float size,
                          const float amplitude,
                          const float period )
{
    std::vector<vec3> positions;
    for (auto p : GeneratePoissonPoints(vertices, 30, false)) {
        float x = size * (p.x - .5f);
        float y = size * (p.y - .5f);
        float z = stb_perlin_noise3(x*period, y*period, 0) * amplitude;
        positions.push_back({x, y, z});
    }
    std::vector<int> indices = triangulate(positions);
    std::vector<vec3> normals = get_normals(positions, indices);

    return MeshDef{positions, indices, normals,
        SURFACE_NORMALS, FRAGMENTED, 1};
}

MeshDef MeshGen::PdpMesh( const int vertices,
                          const float size,
                          const float amplitude,
                          const float period )
{
    std::vector<vec3> positions, normals;

    float d = 0.001f;
    for (auto p : GeneratePoissonPoints(vertices)) {

        float x = size * (p.x - .5f);
        float y = size * (p.y - .5f);
        float z = stb_perlin_noise3(x*period, y*period, 0) * amplitude;
        positions.push_back({x, y, z});

        float z1 = stb_perlin_noise3(x*period, (y+d)*period, 0) * amplitude;
        float z2 = stb_perlin_noise3((x+d)*period, y*period, 0) * amplitude;
        normals.push_back( get_normal({x,y,z}, {x,y+d,z1}, {x+d,y,z2}) );
    }
    std::vector<int> indices = triangulate(positions);

    return MeshDef{positions, indices, normals,
        POSITION_NORMALS, FRAGMENTED, 1};
}

MeshDef MeshGen::Box(float w, float h, float d)
{
    w /= 2, h /= 2, d /= 2;
    std::vector<vec3> positions{
        {-w, -h, -d}, {-w, -h,  d}, {-w,  h, -d}, {-w,  h,  d},
        { w, -h, -d}, { w, -h,  d}, { w,  h, -d}, { w,  h,  d}};
    static std::vector<int> indices{
        0,1,2, 1,3,2,  0,2,6, 0,6,4,  0,4,1, 1,4,5,
        4,6,7, 4,7,5,  1,7,3, 1,5,7,  2,3,7, 2,7,6};
    static std::vector<vec3> normals{
        -unitX, -unitX, -unitZ, -unitZ, -unitY, -unitY,
         unitX,  unitX,  unitZ,  unitZ,  unitY,  unitY};

    return MeshDef{positions, indices, normals,
        SURFACE_NORMALS, FRAGMENTED, 1};
}

MeshDef MeshGen::Sphere(float radius, int n)
{
    auto p_mesh = par_shapes_create_subdivided_sphere(n);
    std::vector<vec3> positions, normals;
    for (int i = 0; i < p_mesh->npoints; i++) {
        vec3 point = radius * vec3{ p_mesh->points[3*i],
                                    p_mesh->points[3*i+1],
                                    p_mesh->points[3*i+2]};
        positions.push_back(point);
        normals.push_back( glm::normalize(point) );
    }

    std::vector<int> indices;
    for (int i = 0; i < p_mesh->ntriangles*3; i++)
        indices.push_back( (int)(p_mesh->triangles[i]) );

    par_shapes_free_mesh(p_mesh);
    return MeshDef{positions, indices, normals,
        POSITION_NORMALS, FRAGMENTED, .02f};
}


