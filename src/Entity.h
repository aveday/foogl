#pragma once

#include <list>
#include <memory>

#include "Mesh.h"
#include "glm.h"

class Entity;
typedef std::list<Entity*> Entity_pl; // unique pointer list

class Entity
{
    private:
        // matrix transform defining the position/scale/rotation of the entity
        Entity_pl children;
        Entity *parent;

    protected:
        void updateModelMatrix();
        vec3 position, scale, rotation;
        Mesh mesh;

    public:
        mat4 modelMatrix, orientation;
        mat4 absModelMatrix();
        Entity(
                GLuint program,
                vec3 pos = vec3(0,0,0),
                vec3 size = vec3(1,1,1)
        );

        void add_child(Entity *child);
        void draw(mat4 &viewMatrix, mat4 &projectionMatrix);
        void rotate(float x, float y, float z);
        void translate(float x, float y, float z);
};
