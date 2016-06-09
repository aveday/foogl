#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include <string>
#include <unordered_map>
#include "glm.h"


typedef class AssetLoader {
public:
    template <typename A> static inline int add_asset(A asset);
    static GLuint LoadProgram(const char* vs, const char* fs);

private:
    //static const int max_assets;
    static GLuint LoadShader(GLenum type, const char *filename);

    template <typename A>
    static std::vector<A> asset_vector; 

    static std::unordered_map<std::string, GLuint>
        program_cache;

} AL;

template <typename A>
int AssetLoader::add_asset(A asset)
{
    if(!asset_vector<A>)
        asset_vector<A> = std::vector<A>();

    asset_vector<A>.puch_back(asset);
    return asset_vector<A>.size()-1;
}

