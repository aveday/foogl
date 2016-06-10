#include <cstdio>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"

void Mesh::draw(mat4 &modelMatrix, vec4 color)
{
    if (vertices_n == 0)
        return;

    // update uniform shader inputs
    glUniformMatrix4fv(UNIFORM_MODEL, 1, 0, glm::value_ptr(modelMatrix));
    glUniform4fv(UNIFORM_COLOR, 1, glm::value_ptr(color));

    // bind vertex array and draw vertices
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertices_n);
    glBindVertexArray(0);
}

Mesh::Mesh(MeshDef &def) : vertices_n(def.vertices_n)
{
    struct { vec3 position, normal; } vertices[vertices_n];
    for(int i = 0; i < vertices_n; i++)
        vertices[i] = { def.positions[def.indices[i]], def.normals[i/6] };

    // create and bind the vertex buffer
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // create and bind the vertex array
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // setup vertex buffers
    glVertexAttribPointer(INPUT_POSITION, 3, GL_FLOAT, 0, 24, (void*)0);
    glVertexAttribPointer(INPUT_NORMAL,   3, GL_FLOAT, 0, 24, (void*)12);

    glEnableVertexAttribArray(INPUT_POSITION);
    glEnableVertexAttribArray(INPUT_NORMAL);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

