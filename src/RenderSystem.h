#pragma once
#include <GL/glew.h>

class RenderSystem {
public:
    void run();

private:
    static const GLuint UNIFORM_MODEL = 1;
    static const GLuint UNIFORM_COLOR = 2;

    static const GLuint UNIFORM_VIEW_MATRIX = 0;
    static const GLuint UNIFORM_VIEW_POSITION = 3;
};
