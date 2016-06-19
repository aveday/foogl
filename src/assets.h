#pragma once
#include <GL/glew.h>
#include <vector>
#include <string>
#include "glm.h"

enum NormalLayout {
    POSITION_NORMAL,
    SURFACE_NORMAL,
    VERTEX_NORMAL
};

struct MeshDef {
    const std::vector<vec3> positions;
    const std::vector<int> indices;
    const std::vector<vec3> normals;
    const NormalLayout normal_layout = SURFACE_NORMAL;
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


