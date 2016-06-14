#pragma once
#include <GL/glew.h>

class LightSystem {
public:
    LightSystem();
    void run();

private:
    inline void update_count(int n);

    const int MAX_LIGHTS = 10;
    int num_lights = 0;
    GLuint ubo;

    float attenuation = 0.2f;
    float ambientCoefficient = 0.005f;

    static const GLuint UNIFORM_BINDING = 0;
};

