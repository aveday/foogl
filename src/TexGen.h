#pragma once
#include <GL/glew.h>

namespace TexGen {
    GLuint MeshNormals(MeshDef &def, ivec2 resolution);
    static const GLuint UNIFORM_VIEW_MATRIX = 0;
}

