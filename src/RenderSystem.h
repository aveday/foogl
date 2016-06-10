#pragma once
#include <GL/glew.h>

class RenderSystem {
public:
    void run();

private:
    static const GLuint UNIFORM_MODEL = 1;
    static const GLuint UNIFORM_COLOR = 2;

    static const GLuint INPUT_POSITION = 0;
    static const GLuint INPUT_NORMAL = 1;
};

