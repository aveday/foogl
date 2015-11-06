#include "Assets.h"


int Assets::Triangles()
{
    int n = 3;
    Vertex Vertices[n*3];

    for(int i = 0; i < n; i++)
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

    return n*3;
}



void split(int s, float **grid)
{
    if(s == 2)
        return;

    float rmin = -0.4f, rmax = 0.4f;
    // calculate lines midpoints
    grid[1*s/4][s/4] = (grid[0  ][0  ] + grid[s/2][s/2]) / 2 + random_f(rmin, rmax);
    grid[2*s/4][s/4] = (grid[s/2][0  ] + grid[s/2][s/2]) / 2 + random_f(rmin, rmax);
    grid[3*s/4][s/2] = (grid[s  ][s/2] + grid[s/2][s/2]) / 2 + random_f(rmin, rmax);

    grid[1*s/4][2*s/4] = (grid[0  ][s/2] + grid[s/2][s/2]) / 2 + random_f(rmin, rmax);
    grid[2*s/4][3*s/4] = (grid[s/2][s  ] + grid[s/2][s/2]) / 2 + random_f(rmin, rmax);
    grid[3*s/4][3*s/4] = (grid[s  ][s  ] + grid[s/2][s/2]) / 2 + random_f(rmin, rmax);

    grid[1*s/4][0*s/4] = (grid[0  ][0  ] + grid[s/2][0  ]) / 2 + random_f(rmin, rmax);
    grid[3*s/4][1*s/4] = (grid[s/2][0  ] + grid[s  ][s/2]) / 2 + random_f(rmin, rmax);
    grid[4*s/4][3*s/4] = (grid[s  ][s/2] + grid[s  ][s  ]) / 2 + random_f(rmin, rmax);

    grid[3*s/4][4*s/4] = (grid[s  ][s  ] + grid[s/2][s  ]) / 2 + random_f(rmin, rmax);
    grid[1*s/4][3*s/4] = (grid[s/2][s  ] + grid[0  ][s/2]) / 2 + random_f(rmin, rmax);
    grid[0*s/4][1*s/4] = (grid[0  ][0  ] + grid[0  ][s/2]) / 2 + random_f(rmin, rmax);

    // construct sub hexes
    float *sw[s/2+1], *se[s/2+1],
           *w[s/2+1],  *e[s/2+1],
          *nw[s/2+1], *ne[s/2+1];

    for(int i = 0; i < s/2+1; i++)
    {
        sw[i] = &grid[i][0];
        se[i] = &grid[i][s/4];
        w[i]  = &grid[i+s/4][0];
        e[i]  = &grid[i+s/4][s/2];
        nw[i] = &grid[i+s/2][s/4];
        ne[i] = &grid[i+s/2][s/2];
    }

    split(s/2, sw);
    split(s/2, se);
    split(s/2, w);
    split(s/2, e);
    split(s/2, nw);
    split(s/2, ne);
}

int Assets::Terrain()
{
    int iter = 6;
    int size = pow(2, iter);

    float max_height = 4.0f;
    float edge = 10.0f;

    float height[size+1][size+1];
    float *grid[size+1];

    for(int s = 0; s <= size; s++)
        grid[s] = height[s];

    for(int y = 0; y <= size; y++)
        for(int x = 0; x <= size; x++)
            grid[y][x] = -1;

    // randomise hex corners
    height[0]     [0     ] = random_f(max_height); // SW
    height[size/2][0     ] = random_f(max_height); // SE
    height[0     ][size/2] = random_f(max_height); // W
    height[size  ][size/2] = random_f(max_height); // E
    height[size/2][size  ] = random_f(max_height); // NW
    height[size  ][size  ] = random_f(max_height); // NE

    // calculate hex midpoint
    grid[size/2][size/2] = (
        grid[0     ][0     ] +
        grid[size  ][size/2] +
        grid[size/2][size  ] +
        grid[0     ][size/2] +
        grid[size/2][0     ] +
        grid[size  ][size  ]) / 6.0f;
        
    split(size, grid);

    // print grid
    for(int y = 0; y <= size; y++)
    {
        for(int n = 0; n < size-y; n++) printf("   ");
        for(int x = 0; x <= size; x++)
        {
            if(grid[y][x] != -1)
                printf("%6.1f, ", grid[y][x]);
            else
                printf("       ");
        }
        printf("\n\n");
    }

    int vs_max = size * size * 2 * 3;
    int vs = 0;

    Vertex Vertices[vs_max];
    glm::vec4 col = glm::vec4(0.0f, 0.5f, 0.0f, 1.0f);

    int dx[][3] = {{0, 1, 1}, {0, 1, 0}};
    int dy[][3] = {{0, 0, 1}, {0, 1, 1}};
    float root3 = sqrt(3);

    for(int y = 0; y < size; y++)
    {
        for(int x = 0; x < size; x++)
        {
            // skip out-of-hex triangles
            if(height[x+1][y+1] == -1)
                continue;
            for(int t = 0; t < 2; t++)
            {
                // skip border triangles
                if(t == 0 && height[x+1][y] == -1
                || t == 1 && height[x][y+1] == -1)
                    continue;

                glm::vec3 pos[3];
                for(int d = 0; d < 3; d++)
                {
                    int X = x+dx[t][d];
                    int Y = y+dy[t][d];
                    pos[d] = glm::vec3(
                            X-Y/2.0f - size/4,
                            height[X][Y],
                            Y*root3/2.0f - size/2 * root3/2);
                }
                // find the normal
                glm::vec3 normal = getNormal(pos);
                
                // add the vertices
                for(int d = 0; d < 3; d++)
                    Vertices[vs++] = Vertex(pos[d], normal, col);

            }
            // calculate the position of each vertex
        }
    }

    printf("%d\n", vs);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    return vs;
}
