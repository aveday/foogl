#pragma once
#include <GL/glew.h>
#include "Component.h"

class RenderSystem {
public:
    void run(CameraC&, GLuint shader);

private:
    static const GLuint UNIFORM_MODEL = 1;
    static const GLuint UNIFORM_COLOR = 2;

    static const GLuint INPUT_POSITION = 0;
    static const GLuint INPUT_NORMAL = 1;

    static const GLuint UNIFORM_CAMERA_MATRIX = 0;
    static const GLuint UNIFORM_CAMERA_POSITION = 3;
};

