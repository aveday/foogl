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

struct MeshDef {
    const int vertices_n;
    const vec3 *positions;
    const int *indices;
    const vec3 *normals;
};

class Mesh {
    public:
        Mesh(const MeshDef&);
        Mesh() : vertices_n(0) {}
        //static Mesh Cube();
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

const vec3 cube_positions[]{
    {-0.5, -0.5, -0.5}, {-0.5, -0.5,  0.5},
    {-0.5,  0.5, -0.5}, {-0.5,  0.5,  0.5},
    { 0.5, -0.5, -0.5}, { 0.5, -0.5,  0.5},
    { 0.5,  0.5, -0.5}, { 0.5,  0.5,  0.5}};
const int cube_indices[]{
    0,1,2, 1,3,2,  0,2,6, 0,6,4,  2,3,7, 2,7,6,
    4,6,7, 4,7,5,  1,7,3, 1,5,7,  0,4,1, 1,4,5};
const vec3 cube_normals[]{
    {-1, 0, 0},    { 0, 0,-1},    { 0, 1, 0},
    { 1, 0, 0},    { 0, 0, 1},    { 0,-1, 0}};

const MeshDef cube_def = { 36, cube_positions, cube_indices, cube_normals };

