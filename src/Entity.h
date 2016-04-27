#pragma once

#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mesh.h"


class Entity
{
    private:
        // matrix transform defining the position/scale/rotation of the entity
        glm::mat4 modelMatrix;
        void updateModelMatrix();

    protected:
        glm::vec3 position, scale, rotation;
        Mesh mesh;

    public:
        Entity(
                GLuint program,
                glm::vec3 pos = glm::vec3(0,0,0),
                glm::vec3 size = glm::vec3(1,1,1)
        );

        void draw(glm::mat4 &viewMatrix);
        void rotate(float yaw, float pitch, float roll);
        void translate(float x, float y, float z);
};
