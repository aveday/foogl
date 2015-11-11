// OpenGL Headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>

// GLM Headers
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

// Standard Headers
#include <iostream>
#include <list>
#include <thread>
#include <chrono>
#include <unistd.h>

// Project Headers
#include "Perlin.h"
#include "Terrain.h"
#include "Shader.h"
#include "Mesh.h"
#include "Entity.h"
#include "config.h"

Terrain *selectedTerrain;

void keyPress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if(action != GLFW_RELEASE)
    {
        int size = selectedTerrain->size;
        float seed = selectedTerrain->seed;
        switch(key)
        {
            case GLFW_KEY_LEFT:
                selectedTerrain->size /= 2;
                break;

            case GLFW_KEY_RIGHT:
                selectedTerrain->size *= 2;
                break;

            case GLFW_KEY_UP:
                selectedTerrain->seed += 0.02f;
                break;

            case GLFW_KEY_DOWN:
                selectedTerrain->seed -= 0.02f;
                break;

            case GLFW_KEY_1:
                selectedTerrain->lac -= 0.1f;
                break;

            case GLFW_KEY_2:
                selectedTerrain->lac += 0.1f;
                break;

            case GLFW_KEY_3:
                selectedTerrain->pers -= 0.01f;
                break;

            case GLFW_KEY_4:
                selectedTerrain->pers += 0.01f;
                break;

            case GLFW_KEY_MINUS:
                selectedTerrain->scale -= 0.1f;
                break;

            case GLFW_KEY_EQUAL:
                selectedTerrain->scale += 0.1f;
                break;

        }
        selectedTerrain->CreateModel();
        std::cout << "Size = " << selectedTerrain->size << std::endl;
    }
}

std::list<Entity*> entities;
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
        (*it)->draw();

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
    Terrain terrain(shaderProgram, 128, glm::vec3(0, 0, -100.0f));
    entities.push_front( &terrain );
    selectedTerrain = &terrain;


    // create timer and fps counter
    float time = glfwGetTime();
    int dtn = 10, dti = 0;
    float dts[dtn];
    float dt_sum = 0;
    for(int i = 0; i < dtn; i++)
        dts[i] = 0;
    int repeatTimer = 0;

    terrain.rotate(0, M_PI/6, 0);

    // set key callbacks
    glfwSetKeyCallback(window, keyPress);

    render(window);
    /* MAIN EVENT LOOP*/
    while(!glfwWindowShouldClose(window))
    {
        // manage time
        if(glfwGetTime() - time < 1/60.0f)
        {
            int ms = 1000 * ( time + 1/60.0f - glfwGetTime() );
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        }
        float newTime = glfwGetTime();
        float dt = newTime - time;
        repeatTimer -= repeatTimer > 0 ? dt : 0;
        time = newTime;

        // calulate average FPS
        if(LOG_FPS)
        {
            dt_sum += dt - dts[dti];
            dts[dti++] = dt;
            dti %= dtn;
            if(dti == 0)
                std::cout << dtn / dt_sum << std::endl;
        }

        // rotate
        terrain.rotate(dt/4, 0, 0);

        // render scene
        render(window);

        // receive input
        glfwPollEvents();
    }

    // clean up
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

