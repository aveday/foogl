#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <math.h>

#include "Entity.h"
#include "glm.h"

Entity::Entity(
        GLuint program,
        vec3 pos,
        vec3 size) :
    position( pos ),
    scale( size ),
    rotation( vec3(0, 0, 0) ),
    mesh(program)
{
    updateModelMatrix();
}

void Entity::add_child(Entity *child)
{
    //std::unique_ptr<Entity> ptr(child);
    children.push_back(child);
}
void Entity::draw(mat4 &viewMatrix, mat4 &projectionMatrix)
{
    // draw model mesh
    mesh.draw(modelMatrix, viewMatrix, projectionMatrix);

    // calculate child view matrix and draw children
    mat4 childViewMatrix = viewMatrix * modelMatrix;
    for(auto it = children.begin(); it != children.end(); it++)
        (*it)->draw(childViewMatrix, projectionMatrix);
}

void Entity::updateModelMatrix()
{
    orientation = mat4();
    orientation = glm::rotate(orientation, glm::radians(rotation.z), vec3(0,0,1));
    orientation = glm::rotate(orientation, glm::radians(rotation.y), vec3(0,1,0));
    orientation = glm::rotate(orientation, glm::radians(rotation.x), vec3(1,0,0));

    // calculate entity model matrix
    modelMatrix = glm::translate<GLfloat>( position )
        * orientation * glm::scale<GLfloat>( scale );
}

void Entity::rotate(float x, float y, float z)
{
    rotation += vec3(x, y, z);
    updateModelMatrix();
}

void Entity::translate(float x, float y, float z)
{
    position += vec3( x, y, z );
    updateModelMatrix();
}