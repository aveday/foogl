#pragma once

#include "Mesh.h"
#include "glm.h"

class Entity
{
    protected:
        Mesh &mesh;
        vec3 position, scale;
        vec4 color;
        vec3 rotation;

        void updateModelMatrix();

    public:
        vec3 heading;
        mat4 modelMatrix, orientation;
        mat4 absModelMatrix();
        Entity(
                Mesh &mesh,
                vec3 pos = vec3(0),
                vec3 size = vec3(1),
                vec4 color = vec4(0)
        );

        void draw();
        void rotate(float x, float y, float z);
        void translate(float x, float y, float z);
        void warp(vec3 pos);

        virtual void update(float dt);
        virtual void updateShader(GLuint shader);
};
