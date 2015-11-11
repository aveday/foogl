#include <math.h>

#include "Terrain.h"
#include "Util.h"

Perlin Terrain::perlin = Perlin(103);
float root3 = sqrt(3);

static glm::vec4 green = glm::vec4(0.0f, 0.4f, 0.0f, 1.0f);
static glm::vec4 white = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
static glm::vec4 brown = glm::vec4(0.2f, 0.1f, 0.02f, 1);
static glm::vec4 yellow= glm::vec4(0.4f, 0.3f, 0.1f, 1);

glm::vec2 gridToWorld(int x, int y, int size)
{
    return 128.0f * glm::vec2(
            (2*x-y) / 2.0f / size - 0.25f,
            y*root3/size/2 - root3/4);
}

Terrain::Terrain(GLuint program, int size_, glm::vec3 pos):
    Entity(program, pos)
{
    size = size_;
    CreateModel();
}

void Terrain::CreateModel()
{
    // limit size
    size = size < 2 ? 2
        : size > 128 ? 128
        : size;

    /*  create terrain heightmap  */

    float height[size+1][size+1];
    for(int x = 0; x <= size; x++)
        for(int y = 0; y <= size; y++)
        {
            //  skip out-of-bounds polygons
            if(y < 1 - size/2 + x - 1|| y > size/2 + x)
                continue;
            glm::vec2 world = gridToWorld(x, y, size);
            float noise = 0;

            float amp = baseAmp;
            float freq = baseFreq;
            for(int i = 0; i < octaves; i++, freq *= lac, amp *= pers)
            {
                noise += perlin.noise(
                        world.x * freq * scale,
                        world.y * freq * scale,
                        seed) * amp / scale;
            }

            height[x][y] = noise + elevation;
        }


    /*  define vertex 3D positions and normalsi  */

    glm::vec3 normals[size+1][size+1];
    glm::vec3 position[size][size][2][3];

    int dx[][3] = {{0, 1, 1}, {0, 1, 0}};
    int dy[][3] = {{0, 0, 1}, {0, 1, 1}};

    for(int y = 0; y < size; y++)
    {
        for(int x = 0; x < size; x++)
        {
            for(int t = 0; t < 2; t++)
            {
                //  skip out-of-bounds polygons
                if(y < 1 - size/2 - t + x || y > size/2 - t + x)
                    continue;

                for(int d = 0; d < 3; d++)
                {
                    // get vertex grid coordinates
                    int X = x+dx[t][d];
                    int Y = y+dy[t][d];

                    // calculate vertex height
                    glm::vec2 pos2d = gridToWorld(X, Y, size);
                    position[x][y][t][d] = glm::vec3(
                            pos2d.x,
                            height[X][Y],
                            pos2d.y);
                }

                // add polygon normal to vertex normal sum
                for(int d = 0; d < 3; d++)
                {
                    int X = x+dx[t][d];
                    int Y = y+dy[t][d];
                    normals[X][Y] += getNormal(position[x][y][t]);
                }
            }
        }
    }


    /*  construct vertex objects  */

    Vertex Vertices[size * size * 2 * 3];
    int vs = 0;

    for(int y = 0; y < size; y++)
    {
        for(int x = 0; x < size; x++)
        {
            for(int t = 0; t < 2; t++)
            {
                //  skip out-of-bounds polygons
                if(y < 1 - size/2 - t + x || y > size/2 - t + x)
                    continue;

                for(int d = 0; d < 3; d++, vs++)
                {
                    // get vertex grid coordinates
                    int X = x+dx[t][d];
                    int Y = y+dy[t][d];

                    // set vertex position
                    Vertices[vs].position = position[x][y][t][d];

                    // set vertex normal
                    Vertices[vs].normal = normalize(normals[X][Y]);

                    // set vertex color
                    float h = (height[X][Y] - elevation) * scale;
                    glm::vec4 water = glm::vec4(0.1f, 0.1f, 0.8f, 1) * (1 + h/12);
                    glm::vec4 &ground = h < 12 ? green : white;
                    float slope = pow(Vertices[vs].normal.y, 10);

                    Vertices[vs].color = h < 0 ? water :
                        h < 0.8f ? yellow :
                        slope * ground + (1 - slope) * brown;
                }
            }
        }
    }

    mesh.begin();
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_DYNAMIC_DRAW);
    mesh.vertices_n = vs;
    mesh.end();
}
