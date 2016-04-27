#include <stdio.h>
#include "Input.h"

void Input::keyPress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if(action != GLFW_RELEASE)
    {
        switch(key)
        {
            // put input here
        }
    }
}
