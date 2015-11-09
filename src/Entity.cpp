#include "Entity.h"

Entity::Entity(GLuint program, glm::vec3 pos) :
    position( pos ),
    scale( glm::vec3(1, 1, 1) ),
    rotation( glm::vec3(0, 0, 0) ),
    mesh(program)
{
    updateModelMatrix();
}

void Entity::draw()
{
    // draw the mesh with the 
    mesh.draw(modelMatrix);
}

void Entity::updateModelMatrix()
{
    // calculate entity model matrix
    modelMatrix
        = glm::translate<GLfloat>( position )
        * glm::rotate<GLfloat>( rotation.x, glm::vec3(1, 0, 0) )
        * glm::rotate<GLfloat>( rotation.y, glm::vec3(0, 1, 0) )
        * glm::rotate<GLfloat>( rotation.z, glm::vec3(0, 0, 1) )
        * glm::scale<GLfloat>( scale );
}

void Entity::rotate(float yaw, float pitch, float roll)
{
    rotation += glm::vec3(pitch, yaw, roll);
    updateModelMatrix();
}

void Entity::translate(float x, float y, float z)
{
    position += glm::vec3( x, y, z );
    updateModelMatrix();
}
