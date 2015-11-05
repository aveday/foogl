#include "Assets.h"


int Assets::Triangles()
{
    Vertex Vertices[9];

    for(int i = 0; i < 3; i++)
    {
        Vertices[i*3] = Vertex(
                glm::vec3( -1.0f +2*i, -1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

        Vertices[i*3+1] = Vertex(
                glm::vec3(1.0f +2*i, -1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

        Vertices[i*3+2] = Vertex(
                glm::vec3(0.0f +2*i,  1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    }

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    return 9;
}

struct tri {
    glm::vec3 &p0, &p1, &p2;
    glm::vec3 m0, m1, m2;
    bool inv;
    tri *parent;
    tri(glm::vec3 &P0, glm::vec3 &P1, glm::vec3 &P2, tri *parent, bool i):
        p0(P0), p1(P1), p2(P2), parent(parent), inv(i) { }

    void split(int iterations, std::vector<Vertex> &verts)
    {
        if(!iterations)
        {
            const glm::vec3 (&p)[] = {p0, p1, p2};
            for(int i = 0; i < 3; i++)
                verts.push_back( Vertex(
                            p[i],
                            glm::vec3(0.0f, 1.0f, 0.0f),
                            glm::vec4(1.0f, 1.0f*i/3, 0.2f*i, 1.0f)) );
        }
        else
        {
            m0 = (p0 + p1) * 0.5f;
            m1 = (p1 + p2) * 0.5f;
            m2 = (p2 + p0) * 0.5f;

            tri children[] = {
                tri(p0, m0, m2, this, false),
                tri(m0, p1, m1, this, false),
                tri(m2, m1, p2, this, false),
                tri(m2, m0, m1, this, true)
            };
            for(int i = 0; i < 4; i++)
                children[i].split(iterations - 1, verts);
        }
    }
};

int Assets::Terrain()
{
    int iterations = 3;
    float edge = 10.0f;

    glm::vec3 p0(0, 0, 0);
    glm::vec3 p1(edge, 0, 0);
    glm::vec3 p2(edge/2, 0, edge*sqrt(3)/2);

    tri root(p0, p1, p2, nullptr, false);
    std::vector<Vertex> verts;

    root.split(iterations, verts);

    Vertex Vertices[verts.size()];
    for(uint i = 0; i < verts.size(); i++)
        Vertices[i] = verts[i];
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    return verts.size();
}
