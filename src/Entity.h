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
        glm::vec3 position, scale, rotation;
        glm::mat4 modelMatrix;
        Mesh mesh;

        void updateModelMatrix();
    
    public:
        Entity(GLuint program, glm::vec3 pos, int (*model)());

        void draw();
        void rotate(float yaw, float pitch, float roll);
        void translate(float x, float y, float z);
};
