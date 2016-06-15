#pragma once
#include "components.h"

class ControlSystem {
public:
    void run();

private:
    static void KeyPress(GLFWwindow* gl_window, int key, int, int action, int);
    static void MouseMove(GLFWwindow* gl_window, double xpos, double ypos);

    static int target;
    static int center_x, center_y;
};

