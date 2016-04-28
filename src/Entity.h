#pragma once

#include <GL/glew.h>
#include <GL/glu.h>
#include "Mesh.h"
#include "glm.h"


class Entity
{
    private:
        // matrix transform defining the position/scale/rotation of the entity
        mat4 modelMatrix;
        void updateModelMatrix();

    protected:
        vec3 position, scale, rotation;
        Mesh mesh;

    public:
        Entity(
                GLuint program,
                vec3 pos = vec3(0,0,0),
                vec3 size = vec3(1,1,1)
        );

        void draw(mat4 &viewMatrix, mat4 &projectionMatrix);
        void rotate(float yaw, float pitch, float roll);
        void translate(float x, float y, float z);
};
