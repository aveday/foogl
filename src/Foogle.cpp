// OpenGL Headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>

// GLM Headers
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

// Standard Headers
#include <stdio.h>
#include <math.h>
#include <list>
#include <algorithm>

// Project Headers
#include "Terrain.h"
#include "Shader.h"
#include "Mesh.h"
#include "Entity.h"
#include "config.h"

std::list<Entity> entities;
GLuint projectionMatrixPtr, lightPositionPtr, lightColorPtr;
glm::mat4 projectionMatrix;

void updateProjection()
{
    float aspect = (float)screen_width / screen_height;
    projectionMatrix = glm::perspective<GLfloat>(fov, aspect, near, far);
    glUniformMatrix4fv(projectionMatrixPtr, 1, GL_FALSE,
            glm::value_ptr(projectionMatrix));
}

void checkSize(GLFWwindow* window)
{
    // get size of OpenGL window
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // recalculate projection matrix
    if (width  != screen_width || height != screen_height)
    {
        screen_height = height;
        screen_width  = width;
        updateProjection();
    }
}

/* RENDER FUNCTION */
void render(GLFWwindow* window) {

    // manage window resizes
    checkSize(window);

    // clear screen
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw entities
    for(auto it = entities.begin(); it != entities.end(); it++)
        it->draw();

    // swap buffers
    glfwSwapBuffers(window);
}

/* MAIN FUNCTION */
int main() {
    
    /* OPENGL CONTEXT CREATION */

    // initialize GLFW and set window options
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, RESIZABLE? GL_TRUE : GL_FALSE);

    // create the window and set it as current context
    GLFWwindow* window = glfwCreateWindow(screen_width, screen_height,
            "Foogle", FULLSCREEN ? glfwGetPrimaryMonitor() : NULL, NULL);
    glfwMakeContextCurrent(window);

    // setup GLEW (must be done after creating GL context)
    glewExperimental = GL_TRUE;
    glewInit();

    // enable depth tests
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);


    /*  GLSL SHADERS  */

    // create the shader program
    GLuint shaderProgram = loadProgram("glsl/flat.vs", "glsl/flat.fs");
    glUseProgram(shaderProgram);

    // setup projection
    projectionMatrixPtr = glGetUniformLocation(shaderProgram, "projection");
    updateProjection();

    // setup lighting
    lightPositionPtr = glGetUniformLocation(shaderProgram, "lightPosition");
    lightColorPtr = glGetUniformLocation(shaderProgram, "lightColor");
    glm::vec3 lightPosition(0, 0, 0);
    glm::vec3 lightColor(1, 1, 1);
    glUniform3f(lightPositionPtr, lightPosition.x, lightPosition.y, lightPosition.z);
    glUniform3f(lightColorPtr, lightColor.x, lightColor.y, lightColor.z);
    
    // create entity
    entities.push_front( Terrain(shaderProgram, glm::vec3(0, 0, -100.0f)) );


    // create timer and fps counter
    float time = glfwGetTime();
    int dtn = 10, dti = 0;
    float dts[dtn];
    float dt_sum = 0;
    for(int i = 0; i < dtn; i++)
        dts[i] = 0;

    entities.begin()->rotate(0, M_PI/6, 0);

    /* MAIN EVENT LOOP*/
    while(!glfwWindowShouldClose(window))
    {
        // manage time
        float newTime = glfwGetTime();
        float dt = newTime - time;
        time = newTime;

        // calulate average FPS
        dt_sum += dt - dts[dti];
        dts[dti++] = dt;
        dti %= dtn;
        if(dti == 0)
            printf("%5.0f\n", dtn / dt_sum);

        // rotate
        entities.begin()->rotate(dt/2, 0, 0);

        // render scene
        render(window);

        // receive input
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // clean up
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

