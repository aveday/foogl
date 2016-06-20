#include "MeshGen.h"
#include "assets.h"

#define PAR_SHAPES_IMPLEMENTATION
#include "par_shapes.h"
#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"
#include "triangulate.h"
#include "poisson.h"

MeshDef MeshGen::PdpDisk( int corners = 1000,
                          float radius = 3.0f,
                          float amplitude = 0.2f,
                          float period = 4 )
{
    std::vector<vec3> positions, normals;

    float d = 0.001f;
    for (auto p : GeneratePoissonPoints(corners)) {
        float x = radius * (p.x - .5f);
        float z = radius * (p.y - .5f);
        float y = stb_perlin_noise3(x*period, 0, z*period) * amplitude;
        float yx = stb_perlin_noise3(x*period, 0, (z+d)*period) * amplitude;
        float yz = stb_perlin_noise3((x+d)*period, 0, z*period) * amplitude;

        positions.push_back({ x, y, z});
        normals.push_back( get_normal({x,y,z}, {(x+d),yx,z}, {x,yz,(z+d)}) );
    }

    std::vector<int> indices = triangulate(positions);

    return MeshDef{positions, indices, normals, POSITION_NORMAL};
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

    return MeshDef{positions, indices, normals};
}

MeshDef MeshGen::Sphere(float radius, int n, uint16_t flags)
{
    par_shapes_mesh *p_mesh = par_shapes_create_subdivided_sphere(n);

    std::vector<vec3> positions, normals;
    bool inv = flags & INVERTED;
    for (int i = 0; i < p_mesh->npoints; i++) {
        vec3 point = radius * vec3{ p_mesh->points[3*i],
                                    p_mesh->points[3*i+(inv?2:1)],
                                    p_mesh->points[3*i+(inv?1:2)]};
        positions.push_back(point);
        normals.push_back( flags & NULL_NORMALS ? vec3{0,0,0}
                : glm::normalize(point * (inv?-1.f:1.f)));
    }

    std::vector<int> indices;
    for (int i = 0; i < p_mesh->ntriangles*3; i++)
        indices.push_back( (int)(p_mesh->triangles[i]) );

    par_shapes_free_mesh(p_mesh);
    return MeshDef{positions, indices, normals, POSITION_NORMAL, .02f};
}


