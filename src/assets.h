#pragma once
#include <GL/glew.h>
#include <vector>
#include <string>
#include "glm.h"

enum TextureType {
    PROJECTED,
    STRETCHED,
    FRAGMENTED,
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
    const TextureType texture_type;
    const float texture_scale = 1.f;
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

struct MaterialDef {
    std::string diffuse_filename;
    std::string normal_filename;
    std::string specular_filename;
};

struct Material {
    GLuint diffuse;
    GLuint normal;
    GLuint specular;
};

