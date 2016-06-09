#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "EntityManager.h"
#include "Component.h"

class WindowSystem {
public:
    WindowSystem(WindowC &root_window);
    void run();
    void Display();

    void MakeWindow(WindowC &window);
private:
    void Clear();
    bool HasResized(WindowC &window);
    static void KeyPress(
            GLFWwindow* gl_window,
            int key, int scancode, int action, int mods);
};

