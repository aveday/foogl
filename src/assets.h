#pragma once
#include <GL/glew.h>
#include <vector>
#include <string>
#include "glm.h"

struct MeshDef {
    const std::vector<vec3> positions;
    const std::vector<int> indices;
    const std::vector<vec3> normals;
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


