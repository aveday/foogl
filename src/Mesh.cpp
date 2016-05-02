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

    // vertex positions
    vec3 position[8];

    int n = 0;
    for(float x = -0.5f; x <= 0.5f; x += 1)
    for(float y = -0.5f; y <= 0.5f; y += 1)
    for(float z = -0.5f; z <= 0.5f; z += 1)
        position[n++] = vec3(x, y, z);

    Vertex Vertices[36];

    int indices[] = {
        0,1,2, //left
        1,3,2,
        0,2,6, //front
        0,6,4,
        2,3,7, //top
        2,7,6,
        0,4,1, //bottom
        1,4,5,
        1,7,3, //back
        1,5,7,
        4,6,7, //right
        4,7,5
    };
    vec3 normals[] = {
        vec3(-1, 0, 0),
        vec3( 0, 0,-1),
        vec3( 0, 1, 0),
        vec3( 0,-1, 0),
        vec3( 0, 0, 1),
        vec3( 1, 0, 0),
    };

    int vs = 0;
    for(int i = 0; i < 36; i++)
    {
        Vertices[vs].position = position[indices[i]];
        Vertices[vs].normal = normals[i/6];
        Vertices[vs].color = vec4(1);
        vs++;
   }

    mesh.begin();
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    mesh.vertices_n = vs;
    mesh.end();
    return mesh;
}

