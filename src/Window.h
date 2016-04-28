#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <list>

#include "Camera.h"
#include "Entity.h"

class Window
{
    public:
        Window(const char* title, int width, int height, bool fullscreen, bool resizable);
        void render(Camera &camera, std::list<Entity*> entities);
        void control(Camera &camera);
        bool hasResized();
        bool running();

    private:
        GLFWwindow* window;
        int width, height;
};

