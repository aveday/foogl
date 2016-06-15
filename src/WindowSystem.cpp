#include <iostream>
#include <thread>
#include <GL/glew.h>
#include "WindowSystem.h"
#include "EntityManager.h"

void WindowSystem::run()
{
    static int init = false;
    if (!init) {
        init = true;
        // initialize GLFW and set window options
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    }

    Window &window = EM::first_component<Window>();
    Clock &clock = EM::first_component<Clock>();

    //TODO check resize
    if (!window.gl_window) MakeWindow(window);
    else glfwMakeContextCurrent(window.gl_window);

    glfwSwapBuffers(window.gl_window);

    // Manage time
    float excess_seconds = clock.time - glfwGetTime() + clock.min;
    if (excess_seconds > 0) {
        int ms = 1000 * excess_seconds;
        std::this_thread::sleep_for( std::chrono::milliseconds(ms) );
    }
    float newTime = glfwGetTime();
    clock.dt = newTime - clock.time;
    clock.time = newTime;

    // clear screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // check for close
    if(glfwWindowShouldClose(window.gl_window))
    {
        glfwDestroyWindow(window.gl_window);
        glfwTerminate();
        clock.running = false;
    }
}

void WindowSystem::MakeWindow(Window &window)
{
    // create the window and set it as current context
    glfwWindowHint(GLFW_RESIZABLE, window.resizable);
    window.gl_window = glfwCreateWindow(
            window.width, window.height, window.title,
            window.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
    glfwMakeContextCurrent(window.gl_window);

    // setup GLEW (must be done after creating GL context)
    static bool first_window = true;
    if (first_window) {
        first_window = false;
        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if (err != GLEW_OK) std::cerr << glewGetErrorString(err);
    }
}

bool WindowSystem::HasResized(Window &window)
{
    // get size of OpenGL window
    int new_width, new_height;
    glfwGetWindowSize(window.gl_window, &new_width, &new_height);
    glViewport(0, 0, new_width, new_height);

    // recalculate projection matrix
    if (new_width  != window.width || new_height != window.height) {
        window.height = new_height;
        window.width  = new_width;
        return true;
    }
    return false;
}

