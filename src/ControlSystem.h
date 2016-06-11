#pragma once
#include "components.h"

class ControlSystem {
public:
    ControlSystem(Window &window);
    void run(int target);
    static void KeyPress(GLFWwindow* gl_window,
            int key, int scancode, int action, int mods);

private:
    static int target;
};

