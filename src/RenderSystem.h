#pragma once
#include <GL/glew.h>

class RenderSystem {
public:
    void run();
private:
    static const GLuint UNIFORM_MODEL = 1;

    static const GLuint UNIFORM_VIEW_MATRIX = 0;
    static const GLuint UNIFORM_VIEW_POSITION = 2;

    static const GLuint UNIFORM_DIFFUSEMAP = 3;
    static const GLuint UNIFORM_NORMALMAP = 4;
    static const GLuint UNIFORM_SPECULARMAP = 5;

	GLuint MatrixID;
	GLuint ModelMatrixID;
	GLuint ViewMatrixID;
	GLuint ModelView3x3MatrixID;
};

