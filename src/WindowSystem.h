#pragma once
#include <GLFW/glfw3.h>
#include "components.h"

class WindowSystem {
public:
    WindowSystem(Window &root_window);
    void run(Window &window, Clock &clock);
    void Display();

    void MakeWindow(Window &window);
private:
    void Clear();
    bool HasResized(Window &window);
    static void KeyPress(
            GLFWwindow* gl_window,
            int key, int scancode, int action, int mods);
};

