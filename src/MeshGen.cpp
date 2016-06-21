#include "MeshGen.h"
#include "assets.h"

#define PAR_SHAPES_IMPLEMENTATION
#include "par_shapes.h"
#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"
#include "triangulate.h"
#include "poisson.h"

MeshDef MeshGen::PdpMesh( const int vertices = 1000,
                          const float size = 3.0f,
                          const float amplitude = 0.2f,
                          const float period = 4,
                          const MeshFlags flags )
{
    std::vector<vec3> positions, normals;
    NormalType ntype = flags & FLAT ? SURFACE_NORMALS : POSITION_NORMALS;

    float d = 0.001f;
    for (auto p : GeneratePoissonPoints(vertices, 30, flags & CIRCULAR)) {

        float x = size * (p.x - .5f);
        float z = size * (p.y - .5f);
        float y = stb_perlin_noise3(x*period, 0, z*period) * amplitude;
        positions.push_back({ x, y, z});

        if (ntype == POSITION_NORMALS) {
            float yx = stb_perlin_noise3(x*period,0,(z+d)*period) * amplitude;
            float yz = stb_perlin_noise3((x+d)*period,0,z*period) * amplitude;
            normals.push_back( get_normal( {x,y,z}, {x+d,yx,z}, {x,yz,z+d}) );
        }
    }
    std::vector<int> indices = triangulate(positions);

    if (ntype == SURFACE_NORMALS)
        normals = get_normals(positions, indices);

    return MeshDef{positions, indices, normals, ntype, 1, flags};
}

MeshDef MeshGen::Box(float w, float h, float d, MeshFlags flags)
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

    return MeshDef{positions, indices, normals, SURFACE_NORMALS, 1, flags};
}

MeshDef MeshGen::Sphere(float radius, int n, MeshFlags flags)
{
    NormalType ntype = flags & REMOVE_NORMALS ? NO_NORMALS : POSITION_NORMALS;
    bool inverted = flags & INVERTED;

    auto p_mesh = par_shapes_create_subdivided_sphere(n);
    std::vector<vec3> positions, normals;
    for (int i = 0; i < p_mesh->npoints; i++) {
        vec3 point = radius * vec3{ p_mesh->points[3*i],
                                    p_mesh->points[3*i+(inverted?2:1)],
                                    p_mesh->points[3*i+(inverted?1:2)]};
        positions.push_back(point);
        if (ntype == POSITION_NORMALS)
            normals.push_back( glm::normalize(point * (inverted?-1.f:1.f)) );
    }

    std::vector<int> indices;
    for (int i = 0; i < p_mesh->ntriangles*3; i++)
        indices.push_back( (int)(p_mesh->triangles[i]) );

    par_shapes_free_mesh(p_mesh);
    return MeshDef{positions, indices, normals, ntype, .02f, flags};
}


