#pragma once

#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec4 color;
    Vertex() { }
    Vertex(glm::vec3 p, glm::vec3 n, glm::vec4 c):
        position(p), normal(n), color(c) {}
};


class Mesh {
    public:
        Mesh(GLuint program, int (*model)());
        void draw(glm::mat4 &modelMatrix);

    private:
        GLuint vao, vbo;
        int vertices_n;
        GLuint shaderProgram;

        void initVertexAttrib(GLuint program, const GLchar* attribName,
                int size, int stride, int offset);

};

