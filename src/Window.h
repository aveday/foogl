#pragma once

#include <GLFW/glfw3.h>
#include "Input.h"

class Window
{
    public:
        Window(const char* title, int width, int height, bool fullscreen, bool resizable);
        bool hasResized();
        bool stillOpen();
        void display();
        void close();

    private:
        GLFWwindow* window;
        int width, height;
        Input input;
};

