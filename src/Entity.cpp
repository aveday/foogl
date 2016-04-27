#include "Entity.h"

Entity::Entity(
        GLuint program,
        glm::vec3 pos,
        glm::vec3 size) :
    position( pos ),
    scale( size ),
    rotation( glm::vec3(0, 0, 0) ),
    mesh(program)
{
    updateModelMatrix();
}

void Entity::draw(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix)
{
    // draw the mesh with the 
    mesh.draw(modelMatrix, viewMatrix, projectionMatrix);
}

void Entity::updateModelMatrix()
{
    // calculate entity model matrix
    modelMatrix
        = glm::translate<GLfloat>( position )
        //FIXME there's probably a better way to do this
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
