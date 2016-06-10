#pragma once
#include <GL/glew.h>
#include "glm.h"

struct MeshDef {
    const int vertices_n;
    const vec3 *positions;
    const int *indices;
    const vec3 *normals;
};

class Mesh {
    public:
        Mesh(MeshDef&);
        Mesh() : vertices_n(0) {}
        void draw(mat4 &modelMatrix, vec4 color = vec4());
        int vertices_n = 0;

    private:
        GLuint vao, vbo;

        void initVertexAttrib(const GLchar* attribName,
                int size, int stride, int offset);

        static const GLuint UNIFORM_MODEL = 1;
        static const GLuint UNIFORM_COLOR = 2;

        static const GLuint INPUT_POSITION = 0;
        static const GLuint INPUT_NORMAL = 1;
};
