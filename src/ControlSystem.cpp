#include <iostream>
#include "EntityManager.h"
#include "ControlSystem.h"
#include "components.h"
#include "glm.h"

int ControlSystem::target;
int ControlSystem::center_x;
int ControlSystem::center_y;

void ControlSystem::run()
{
    Window &window = EM::first_component<Window>();
    target = EM::first_entity<Controller>();
    static bool init = false;
    if (!init) {
        init = true;
        glfwSetInputMode(window.gl_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        glfwSetKeyCallback(window.gl_window, KeyPress);
        glfwSetCursorPosCallback(window.gl_window, MouseMove);
        //glfwSetMouseButtonCallback(window.gl_window, MousePress);

        int width, height;
        glfwGetWindowSize(window.gl_window, &width, &height);
        center_x = width/2;
        center_y = height/2;
    }
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
        }
    } else if (action == GLFW_RELEASE) {
        switch(key) {
            case GLFW_KEY_W: body.velocity.z += controller.speed; break;
            case GLFW_KEY_A: body.velocity.x += controller.speed; break;
            case GLFW_KEY_S: body.velocity.z -= controller.speed; break;
            case GLFW_KEY_D: body.velocity.x -= controller.speed; break;
        }
    }
}

void ControlSystem::MouseMove(GLFWwindow* gl_window, double xpos, double ypos)
{
    Controller &controller = EM::get_component<Controller>(target);
    Body &body = EM::get_component<Body>(target);

    body.rotation += vec3{center_y - ypos, center_x - xpos, 0}
                   * controller.mouse_sensitivity;

    euler_normalise(body.rotation);
    set_transform(body.transform, body.position, body.scale, body.rotation);

    glfwSetCursorPos(gl_window, center_x, center_y);
}

