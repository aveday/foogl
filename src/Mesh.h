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
        Mesh(GLuint program);
        void draw(mat4 &modelMatrix);
        void begin();
        void end();
        int vertices_n;

    private:
        GLuint vao, vbo;
        GLuint shaderProgram;

        void initVertexAttrib(GLuint program, const GLchar* attribName,
                int size, int stride, int offset);

};

