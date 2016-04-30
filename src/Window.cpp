#include "Window.h"

const float mouseSensitivity = 0.05f;

void Window::keyPress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Entity *entity = static_cast<Entity*>(glfwGetWindowUserPointer(window));

    if(key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, GL_TRUE);


    float speed = 1.0f;
    if(action == GLFW_PRESS)
    {
        switch(key)
        {
            case GLFW_KEY_W: entity->heading.z -= speed; break;
            case GLFW_KEY_A: entity->heading.x -= speed; break;
            case GLFW_KEY_S: entity->heading.z += speed; break;
            case GLFW_KEY_D: entity->heading.x += speed; break;
        }
    }
    else if(action == GLFW_RELEASE)
    {
        switch(key)
        {
            case GLFW_KEY_W: entity->heading.z += speed; break;
            case GLFW_KEY_A: entity->heading.x += speed; break;
            case GLFW_KEY_S: entity->heading.z -= speed; break;
            case GLFW_KEY_D: entity->heading.x -= speed; break;
        }
    }
}

Window::Window(const char* title,
        int w, int h,
        bool fullscreen, bool resizable):
    width(w), height(h)
{
    // initialize GLFW and set window options
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, resizable ? GL_TRUE : GL_FALSE);

    // create the window and set it as current context
    window = glfwCreateWindow(width, height,
            title, fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
    glfwMakeContextCurrent(window);

    // setup GLEW (must be done after creating GL context)
    glewExperimental = GL_TRUE;
    glewInit();

    // enable depth tests
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // set key callbacks
    glfwSetKeyCallback(window, keyPress);

    // hide mouse and move to the middle of the screen
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

bool Window::hasResized()
{
    // get size of OpenGL window
    int new_width, new_height;
    glfwGetFramebufferSize(window, &new_width, &new_height);
    glViewport(0, 0, new_width, new_height);

    // recalculate projection matrix
    if (new_width  != width || new_height != height)
    {
        height = new_height;
        width  = new_width;
        return true;
    }
    return false;
}

void Window::render(Camera &camera, std::list<Entity*> entities)
{
    // update aspect ratio for camera projection when resized
    if(hasResized())
        camera.updateProjection((float)width / height);

    // clear screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw entities
    for(auto entity : entities)
        entity->draw();

    glfwSwapBuffers(window);
}

void Window::control(Entity &entity)
{
    //TODO reimplement with glfw mouse callback function
    // rotate camera based on mouse movement
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    entity.rotate(
            -mouseSensitivity * (mouseY - height / 2.0f),
            -mouseSensitivity * (mouseX - width / 2.0f),
            0);

    // reset cursor to center
    glfwSetCursorPos(window, width / 2, height / 2);

    // poll for keyboard input
    glfwPollEvents();

    // set the window's user pointer to the camera
    glfwSetWindowUserPointer(window, (void*)&entity);
}

bool Window::running()
{
    if(glfwWindowShouldClose(window))
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }
    return true;
}
