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
    if (!EM::has_components<Body, Controller>(target)) {
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
    Body &body = EM::get_component<Body>(target);

    if (key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(gl_window, GL_TRUE);

    if (action == GLFW_PRESS) {
        switch(key) {
            case GLFW_KEY_W: body.velocity.z -= controller.speed; break;
            case GLFW_KEY_A: body.velocity.x -= controller.speed; break;
            case GLFW_KEY_S: body.velocity.z += controller.speed; break;
            case GLFW_KEY_D: body.velocity.x += controller.speed; break;
        default: break;
        }
    } else if (action == GLFW_RELEASE) {
        switch(key) {
            case GLFW_KEY_W: body.velocity.z += controller.speed; break;
            case GLFW_KEY_A: body.velocity.x += controller.speed; break;
            case GLFW_KEY_S: body.velocity.z -= controller.speed; break;
            case GLFW_KEY_D: body.velocity.x -= controller.speed; break;
        default: break;
        }
    }
}

void ControlSystem::MouseMove(GLFWwindow* gl_window, double xpos, double ypos)
{
    Controller &controller = EM::get_component<Controller>(target);
    Body &body = EM::get_component<Body>(target);

    double xcenter = 1024/2, ycenter = 768/2; //FIXME
    float x_rotation = controller.mouse_sensitivity * (ycenter - ypos);
    float y_rotation = controller.mouse_sensitivity * (xcenter - xpos);

    body.rotation.y = fmodf(body.rotation.y + y_rotation, 2*M_PI);
    body.rotation.x = glm::clamp(body.rotation.x + x_rotation,
            -(float)M_PI_2, (float)M_PI_2);

    body.transform = get_transform(body.position, body.scale, body.rotation);

    glfwSetCursorPos(gl_window, xcenter, ycenter);
}

