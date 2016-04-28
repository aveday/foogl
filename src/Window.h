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
        void update(std::list<Entity*> entities, float dt);
        void control(Entity &entity);
        bool hasResized();
        bool running();

    private:
        static void keyPress(GLFWwindow* window, int key, int scancode, int action, int mods);
        GLFWwindow* window;
        int width, height;
};

