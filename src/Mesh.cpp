#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "glm.h"

Mesh::Mesh(GLuint program) :
    shaderProgram(program)
{ }

void Mesh::begin()
{
    // create and bind the vertex buffer
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // create and bind the vertex array
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

void Mesh::end()
{
    // setup vertex buffers
    initVertexAttrib(shaderProgram, "Position", 3, 10, 0);
    initVertexAttrib(shaderProgram, "Normal", 3, 10, 3);
    initVertexAttrib(shaderProgram, "Color", 4, 10, 6);

    glBindVertexArray(0);
}


void Mesh::initVertexAttrib(GLuint program, const GLchar* attribName,
        int size, int stride, int offset)
{
    GLuint attrib = glGetAttribLocation(program, attribName);
    glVertexAttribPointer(attrib, size, GL_FLOAT, GL_FALSE,
            stride * sizeof(GLfloat), (const void*)(sizeof(GLfloat) * offset));
    glEnableVertexAttribArray(attrib);
}

void Mesh::draw(mat4 &modelMatrix, vec4 color)
{
    if (vertices_n == 0)
        return;

    glUseProgram(shaderProgram);
    // update uniform shader inputs
    glUniformMatrix4fv(UNIFORM_MODEL, 1, 0, glm::value_ptr(modelMatrix));
    glUniform4fv(UNIFORM_COLOR, 1, glm::value_ptr(color));

    // bind vertex array and draw vertices
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertices_n);
    glBindVertexArray(0);
}

Mesh Mesh::Cube(GLuint shader)
{
    Mesh mesh(shader);

    mesh.vertices_n = 36;

    vec3 positions[]{
        {-0.5, -0.5, -0.5}, {-0.5, -0.5,  0.5},
        {-0.5,  0.5, -0.5}, {-0.5,  0.5,  0.5},
        { 0.5, -0.5, -0.5}, { 0.5, -0.5,  0.5},
        { 0.5,  0.5, -0.5}, { 0.5,  0.5,  0.5}};
    int indices[]{
        0,1,2, 1,3,2,  0,2,6, 0,6,4,  2,3,7, 2,7,6,
        4,6,7, 4,7,5,  1,7,3, 1,5,7,  0,4,1, 1,4,5};
    vec3 normals[]{
        {-1, 0, 0},    { 0, 0,-1},    { 0, 1, 0},
        { 1, 0, 0},    { 0, 0, 1},    { 0,-1, 0}};

    struct { vec3 position, normal; vec4 color; } vertices[mesh.vertices_n];
    for(int i = 0; i < mesh.vertices_n; i++)
        vertices[i] = { positions[indices[i]], normals[i/6], vec4(1) };

    // creat and fill array and buffers
    mesh.begin();
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    mesh.end();

    return mesh;
}

