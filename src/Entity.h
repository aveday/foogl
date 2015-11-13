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
        glm::mat4 modelMatrix;
        void updateModelMatrix();

    
    public:
        glm::vec3 position, scale, rotation;
        Entity(GLuint program, glm::vec3 pos);
        Mesh mesh;

        void draw(glm::mat4 &viewMatrix);
        void rotate(float yaw, float pitch, float roll);
        void translate(float x, float y, float z);
};
