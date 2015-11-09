#include "Assets.h"

int Assets::Terrain()
{

    int iter = 7;
    int size = pow(2, iter);
    float height[size+1][size+1];

    // Perlin noise parameters
    Perlin p(43);
    int levels = 3;
    float xScale[] = {0.6f, 0.3f, 0.1f};
    float yScale[] = {0.4f, 0.03f, 0.02f};
    float elevation = 2.0f;
    float root3 = sqrt(3);

    // generate heightmap from Perlin noise
    for(int x = 0; x <= size; x++)
        for(int y = 0; y <= size; y++)
        {
            if(y < 1 - size/2 + x - 1|| y > size/2 + x)
                continue;
            float xf = x-y/2.0f - size/4;
            float yf = y*root3/2.0f - size/2 * root3/2;
            float noise = 0;
            for(int i = 0; i < levels; i++)
                noise += p.noise(
                        xf / size / xScale[i],
                        yf / size / xScale[i],
                        i) * size * yScale[i];

            height[x][y] = noise + elevation;
        }

    glm::vec3 normals[size+1][size+1];
    glm::vec3 pos[size][size][2][3];

    Vertex Vertices[size * size * 2 * 3];
    int vs = 0;

    int dx[][3] = {{0, 1, 1}, {0, 1, 0}};
    int dy[][3] = {{0, 0, 1}, {0, 1, 1}};

    for(int y = 0; y < size; y++)
    {
        for(int x = 0; x < size; x++)
        {
            for(int t = 0; t < 2; t++)
            {
                //  crop out-of-bounds areas
                if(y < 1 - size/2 - t + x || y > size/2 - t + x)
                    continue;

                // calculate vertex positions
                for(int d = 0; d < 3; d++)
                {
                    int X = x+dx[t][d];
                    int Y = y+dy[t][d];

                    float xf = X-Y/2.0f - size/4;
                    float yf = Y*root3/2.0f - size/2 * root3/2;
                    pos[x][y][t][d] = glm::vec3(
                            xf,
                            height[X][Y],
                            yf);
                }

                for(int d = 0; d < 3; d++)
                {
                    int X = x+dx[t][d];
                    int Y = y+dy[t][d];
                    normals[X][Y] += getNormal(pos[x][y][t]);
                }
            }
        }
    }

    for(int y = 0; y < size; y++)
    {
        for(int x = 0; x < size; x++)
        {
            for(int t = 0; t < 2; t++)
            {
                for(int d = 0; d < 3; d++, vs++)
                {
                    // get vertex grid coordinates
                    int X = x+dx[t][d];
                    int Y = y+dy[t][d];

                    // set vertex position
                    Vertices[vs].position = pos[x][y][t][d];

                    // set vertex normal
                    Vertices[vs].normal = normalize(normals[X][Y]);

                    // set vertex color
                    glm::vec4 water = glm::vec4(0.1f, 0.1f, 0.8f, 1) * (1+height[X][Y]/12);
                    glm::vec4 &ground = height[X][Y] < 12 ? green : white;
                    float slope = pow(Vertices[vs].normal.y, 10);

                    Vertices[vs].color = height[X][Y] < 0 ? water :
                        height[X][Y] < 0.8f ? yellow :
                        slope * ground + (1 - slope) * brown;
                }
            }
        }
    }

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    return vs;
}
