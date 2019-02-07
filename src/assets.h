#pragma once
#include <GL/glew.h>
#include <vector>
#include <string>
#include "glm.h"

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

