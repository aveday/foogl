#pragma once
#include <GL/glew.h>
#include <vector>
#include <string>
#include "glm.h"


typedef uint8_t MeshFlags;
enum {
    INVERTED       = 1<<0,
    CIRCULAR       = 1<<1,
    FLAT           = 1<<2,
    REMOVE_NORMALS = 1<<3,
};

enum NormalType {
    NO_NORMALS,
    POSITION_NORMALS,
    SURFACE_NORMALS,
    VERTEX_NORMALS
};

struct MeshDef {
    const std::vector<vec3> positions;
    const std::vector<int> indices;
    const std::vector<vec3> normals;
    const NormalType normal_type;
    const float texture_scale = 1.f;
    const MeshFlags flags = 0;
};

struct Mesh {
    int vertices_n = 0;
    GLuint vao = -1;
    GLuint vbo = -1;
};

struct ShaderDef {
    std::string vs_filename;
    std::string fs_filename;
};


