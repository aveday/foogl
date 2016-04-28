#include "Box.h"
#include "glm.h"

Box::Box(GLuint program, vec3 size, vec3 pos, vec4 color):
    Entity(program, pos, size)
{

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
        Vertices[vs].color = color;
        vs++;
   }

    mesh.begin();
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    mesh.vertices_n = vs;
    mesh.end();
}
