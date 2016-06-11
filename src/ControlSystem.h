#pragma once
#include "components.h"

class ControlSystem {
public:
    ControlSystem(Window &window);
    void run(int target);
    static void KeyPress(GLFWwindow* gl_window, int key, int, int action, int);
    static void MouseMove(GLFWwindow* gl_window, double xpos, double ypos);

private:
    static int target;
};

