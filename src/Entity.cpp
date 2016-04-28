#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

void Entity::draw(mat4 &viewMatrix, mat4 &projectionMatrix)
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
        * glm::rotate<GLfloat>( rotation.x, vec3(1, 0, 0) )
        * glm::rotate<GLfloat>( rotation.y, vec3(0, 1, 0) )
        * glm::rotate<GLfloat>( rotation.z, vec3(0, 0, 1) )
        * glm::scale<GLfloat>( scale );
}

void Entity::rotate(float yaw, float pitch, float roll)
{
    rotation += vec3(pitch, yaw, roll);
    updateModelMatrix();
}

void Entity::translate(float x, float y, float z)
{
    position += vec3( x, y, z );
    updateModelMatrix();
}
