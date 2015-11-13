#include "Mesh.h"

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

void Mesh::draw(glm::mat4 &modelMatrix, glm::mat4 &viewMatrix)
{
    glm::mat4 modelview = viewMatrix * modelMatrix;

    // update uniform shader inputs
    glUniformMatrix4fv(
            glGetUniformLocation(shaderProgram, "modelview"),
            1, GL_FALSE, glm::value_ptr(modelview));

    // bind vertex array and draw vertices
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertices_n);
    glBindVertexArray(0);
}


