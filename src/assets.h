#pragma once
#include <string>
#include "glm.h"

struct MeshDef {
    const int vertices_n;
    const vec3 *positions;
    const int *indices = nullptr;
    const vec3 *normals = nullptr;
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

