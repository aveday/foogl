#pragma once
#include <GL/glew.h>

namespace TexGen {
    GLuint MeshNormals(MeshDef &def, ivec2 resolution);

    //FIXME
    static const GLuint UNIFORM_MODEL = 1;
    static const GLuint UNIFORM_TEXTURE = 2;
    static const GLuint UNIFORM_VIEW_MATRIX = 0;
    static const GLuint UNIFORM_VIEW_POSITION = 3;
}

