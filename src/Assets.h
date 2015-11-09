#pragma once

#include "Perlin.h"
#include "Mesh.h"
#include "Util.h"

static glm::vec4 green = glm::vec4(0.0f, 0.4f, 0.0f, 1.0f);
static glm::vec4 white = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
static glm::vec4 brown = glm::vec4(0.2f, 0.1f, 0.02f, 1);
static glm::vec4 yellow= glm::vec4(0.4f, 0.3f, 0.1f, 1);

class Assets
{
    public:
        static int Terrain();
};
