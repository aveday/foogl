#pragma once
#include <GL/glew.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "glm.h"
#include "assets.h"


typedef class AssetLoader {
public:
    template <typename A> static inline int add_asset(A asset);
    static GLuint LoadProgram(const char* vs, const char* fs);
    static Mesh LoadMesh(MeshDef &def);

private:
    static GLuint LoadShader(GLenum type, const char *filename);

    static std::unordered_map<std::string, GLuint> program_cache;
    static std::unordered_map<MeshDef*, Mesh> mesh_cache;

    static const GLuint INPUT_POSITION = 0;
    static const GLuint INPUT_NORMAL = 1;
} AL;

