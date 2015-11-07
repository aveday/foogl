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

void split(int s, float **g, float diff)
{
    int h = s/2;
    int q = s/4;

    float rmax = 0.05f * s;
    float rmin = -rmax;

    bool corners = true;
    bool mids = true;
    bool edges = true;

    if(corners)
    {
        g[0][0] += diff/20; //= (diff + g[0*h][0*h]) / 2;
        g[h][0] += diff/20; //= (diff + g[1*h][0*h]) / 2;
        g[s][h] += diff/20; //= (diff + g[2*h][1*h]) / 2;
        g[s][s] += diff/20; //= (diff + g[2*h][2*h]) / 2;
        g[h][s] += diff/20; //= (diff + g[1*h][2*h]) / 2;
        g[0][h] += diff/20; //= (diff + g[0*h][1*h]) / 2;
        g[h][h] += diff/20; //= (diff + g[0*h][1*h]) / 2;
    }

    // cannot adjust mids or edges at n == 2
    if(s == 2)
        return;

    if(edges)
    {
        g[1*q][0*q] = (g[0][0] + g[h][0]) / 2 + random_f(rmin, rmax);
        g[3*q][1*q] = (g[h][0] + g[s][h]) / 2 + random_f(rmin, rmax);
        g[4*q][3*q] = (g[s][h] + g[s][s]) / 2 + random_f(rmin, rmax);
        g[3*q][4*q] = (g[s][s] + g[h][s]) / 2 + random_f(rmin, rmax);
        g[1*q][3*q] = (g[h][s] + g[0][h]) / 2 + random_f(rmin, rmax);
        g[0*q][1*q] = (g[0][0] + g[0][h]) / 2 + random_f(rmin, rmax);
    }
    if(mids)
    {
        g[1*q][1*q] = (g[0][0] + g[h][h] + g[1*q][0*q] + g[0*q][1*q]) / 4;
        g[2*q][1*q] = (g[h][0] + g[h][h] + g[1*q][0*q] + g[3*q][1*q]) / 4;
        g[3*q][2*q] = (g[s][h] + g[h][h] + g[3*q][1*q] + g[4*q][3*q]) / 4;
        g[1*q][2*q] = (g[0][h] + g[h][h] + g[4*q][3*q] + g[3*q][4*q]) / 4;
        g[2*q][3*q] = (g[h][s] + g[h][h] + g[3*q][4*q] + g[1*q][3*q]) / 4;
        g[3*q][3*q] = (g[s][s] + g[h][h] + g[1*q][3*q] + g[0*q][1*q]) / 4;
    }

    // construct sub hexes
    float *sw[h+1], *se[h+1],
           *w[h+1],  *e[h+1],
          *nw[h+1], *ne[h+1],
           *m[h+1];

    for(int i = 0; i < s/2+1; i++)
    {
        sw[i] = &g[i+0][0];
        se[i] = &g[i+0][q];
        w[i]  = &g[i+q][0];
        e[i]  = &g[i+q][h];
        nw[i] = &g[i+h][q];
        ne[i] = &g[i+h][h];
    }

    split(s/2, sw, diff + random_f(rmin, rmax) );
    split(s/2, ne, diff + random_f(rmin, rmax) );
    split(s/2, nw, diff + random_f(rmin, rmax) );
    split(s/2, se, diff + random_f(rmin, rmax) );
    split(s/2, w,  diff + random_f(rmin, rmax) );
    split(s/2, e,  diff + random_f(rmin, rmax) );

}

int Assets::Terrain()
{
    int iter = 6;
    int size = pow(2, iter);

    float min_height = -2.0f;
    float max_height =  4.0f;

    float height[size+1][size+1];

    float *grid[size+1];

    glm::vec4 green = glm::vec4(0.0f, 0.4f, 0.0f, 1.0f);
    glm::vec4 white = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
    glm::vec4 brown = glm::vec4(0.2f, 0.1f, 0.02f, 1);
    glm::vec4 yellow= glm::vec4(0.4f, 0.3f, 0.1f, 1);

    for(int s = 0; s <= size; s++)
        grid[s] = height[s];

    for(int y = 0; y <= size; y++)
        for(int x = 0; x <= size; x++)
            grid[y][x] = 0;

    // randomise hex corners
    height[0]     [0     ] = random_f(min_height, max_height); // SW
    height[size/2][0     ] = random_f(min_height, max_height); // SE
    height[0     ][size/2] = random_f(min_height, max_height); // W
    height[size  ][size/2] = random_f(min_height, max_height); // E
    height[size/2][size  ] = random_f(min_height, max_height); // NW
    height[size  ][size  ] = random_f(min_height, max_height); // NE

    // calculate hex midpoint
    grid[size/2][size/2] = (
        grid[0     ][0     ] +
        grid[size  ][size/2] +
        grid[size/2][size  ] +
        grid[0     ][size/2] +
        grid[size/2][0     ] +
        grid[size  ][size  ]) / 6.0f;
        
    split(size, grid, 0);

    /*
    // print grid
    for(int y = 0; y <= size; y++)
    {
        for(int n = 0; n < size-y; n++) printf("   ");
        for(int x = 0; x <= size; x++)
        {
            if(grid[y][x] != 0)
                printf("%6.1f, ", grid[y][x]);
            else
                printf("       ");
        }
        printf("\n\n");
    }
    */

    int vs_max = size * size * 2 * 3;
    int vs = 0;

    glm::vec3 normals[size+1][size+1];
    glm::vec3 pos[size+1][size+1][2][3];
    Vertex Vertices[vs_max];

    int dx[][3] = {{0, 1, 1}, {0, 1, 0}};
    int dy[][3] = {{0, 0, 1}, {0, 1, 1}};
    float root3 = sqrt(3);

    for(int y = 0; y < size; y++)
    {
        for(int x = 0; x < size; x++)
        {
            for(int t = 0; t < 2; t++)
            {

                //  crop out-of-bounds areas
                if(y < 1 - size/2 - t + x || y > size/2 - t + x)
                    continue;

                for(int d = 0; d < 3; d++)
                {
                    int X = x+dx[t][d];
                    int Y = y+dy[t][d];

                    pos[x][y][t][d] = glm::vec3(
                            X-Y/2.0f - size/4,
                            height[X][Y],
                            Y*root3/2.0f - size/2 * root3/2);
                }
                for(int d = 0; d < 3; d++)
                {
                    int X = x+dx[t][d];
                    int Y = y+dy[t][d];
                    // find the normal
                    normals[X][Y] = getNormal(pos[x][y][t]);
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
                // determine ground color


                // add the vertices
                for(int d = 0; d < 3; d++)
                {
                    int X = x+dx[t][d];
                    int Y = y+dy[t][d];

                    glm::vec4 water = glm::vec4(0, 0, 1.0f, 1) * (1+height[X][Y]/5);
                    glm::vec4 &ground = height[X][Y] < 5 ? green : white;
                    glm::vec3 n = normalize(normals[X][Y]);

                    float slope = pow(n.y, 10);
                    glm::vec4 col = 
                        height[X][Y] < 0.2f ? yellow :
                        slope * ground + (1 - slope) * brown;

                    Vertices[vs++] = Vertex(pos[x][y][t][d], n, col);
                }

            }
            // calculate the position of each vertex
        }
    }

    printf("%d\n", vs);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    return vs;
}
