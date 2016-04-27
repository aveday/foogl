#pragma once

#include <GLFW/glfw3.h>

struct Input
{
    Input() { };
    static void keyPress(GLFWwindow* window, int key, int scancode, int action, int mods);
};

