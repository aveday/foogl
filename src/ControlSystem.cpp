#include <iostream>
#include "EntityManager.h"
#include "ControlSystem.h"
#include "glm.h"

int ControlSystem::target;

ControlSystem::ControlSystem(Window &window)
{
    glfwSetInputMode(window.gl_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glfwSetKeyCallback(window.gl_window, KeyPress);
    glfwSetCursorPosCallback(window.gl_window, MouseMove);
    //glfwSetMouseButtonCallback(window.gl_window, MousePress);
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
    Transform &transform = EM::get_component<Transform>(target);

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

void ControlSystem::MouseMove(GLFWwindow* gl_window, double xpos, double ypos)
{
    Controller &controller = EM::get_component<Controller>(target);
    Transform &transform = EM::get_component<Transform>(target);

    double xcenter= 1024/2, ycenter= 768/2; //FIXME
    double dx = xpos - xcenter;
    double dy = ypos - ycenter;

    glfwSetCursorPos(gl_window, xcenter, ycenter);
}

