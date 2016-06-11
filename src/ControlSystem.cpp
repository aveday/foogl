#include <iostream>
#include "EntityManager.h"
#include "ControlSystem.h"
#include "glm.h"

int ControlSystem::target;

ControlSystem::ControlSystem(Window &window)
{
    glfwSetKeyCallback(window.gl_window, KeyPress);
}

void ControlSystem::run(int target)
{
    if (!EM::has_components<Controller>(target)) {
        std::cerr << "ControlSystem target needs Controller component\n";
        exit(EXIT_FAILURE);
    }
    this->target = target;
    glfwPollEvents();
}

void ControlSystem::KeyPress(GLFWwindow* gl_window,
        int key, int scancode, int action, int mods)
{
    Controller &controller = EM::get_component<Controller>(target);
    Velocity &velocity = EM::get_component<Velocity>(target);

    if (key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(gl_window, GL_TRUE);

    if (action == GLFW_PRESS) {
        switch(key) {
        case GLFW_KEY_W: velocity.z += controller.speed; break;
        case GLFW_KEY_A: velocity.x -= controller.speed; break;
        case GLFW_KEY_S: velocity.z -= controller.speed; break;
        case GLFW_KEY_D: velocity.x += controller.speed; break;
        default: break;
        }
    } else if (action == GLFW_RELEASE) {
        switch(key) {
        case GLFW_KEY_W: velocity.z -= controller.speed; break;
        case GLFW_KEY_A: velocity.x += controller.speed; break;
        case GLFW_KEY_S: velocity.z += controller.speed; break;
        case GLFW_KEY_D: velocity.x -= controller.speed; break;
        default: break;
        }
    }
}
