#pragma once
#include <GL/glew.h>
#include "glm.h"

struct Vertex {
    vec3 position;
    vec3 normal;
    vec4 color;
    Vertex() { }
    Vertex(vec3 p, vec3 n, vec4 c):
        position(p), normal(n), color(c) {}
};

class Mesh {
    public:
        static Mesh Cube();
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
        static const GLuint INPUT_COLOR = 2;
};

