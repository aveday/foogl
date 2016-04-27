#include <GL/glew.h>
#include <stdio.h>
#include "Window.h"

Window::Window(const char* title,
        int w, int h,
        bool fullscreen, bool resizable):
    width(w), height(h)
{
    /* OPENGL CONTEXT CREATION */

    // initialize GLFW and set window options
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, resizable ? GL_TRUE : GL_FALSE);

    // create the window and set it as current context
    window = glfwCreateWindow(width, height,
            title, fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
    glfwMakeContextCurrent(window);

    // setup GLEW (must be done after creating GL context)
    glewExperimental = GL_TRUE;
    glewInit();

    // enable depth tests
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // set key callbacks
    glfwSetKeyCallback(window, input.keyPress);
}

bool Window::hasResized()
{
    // get size of OpenGL window
    int new_width, new_height;
    glfwGetFramebufferSize(window, &new_width, &new_height);
    glViewport(0, 0, new_width, new_height);

    // recalculate projection matrix
    if (new_width  != width || new_height != height)
    {
        height = new_height;
        width  = new_width;
        return true;
    }
    return false;
}

void Window::display()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool Window::stillOpen()
{
    return !glfwWindowShouldClose(window);
}

void Window::close()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

