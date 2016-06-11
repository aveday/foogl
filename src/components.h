#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm.h"
#include "assets.h"

typedef glm::mat4 Transform;
typedef glm::vec3 Velocity;
typedef glm::vec3 Position;
typedef glm::vec3 Rotation;
typedef glm::vec3 Scale;

struct Window {
    const char* title;
    int width = 1024;
    int height = 768;
    bool fullscreen = false;
    bool resizable = false;
    GLFWwindow* gl_window = nullptr;
};

struct Clock {
    float min;
    float start_time = (float)glfwGetTime(); //FIXME
    float time = 0;
    float dt = 0;
};

struct Light {
    vec3 intensity;
    vec3 position;
    //TODO add attenuation and ambient coefficient
};

struct Camera {
    float fov = 45;
    float aspect = 4.0/3.0f; //FIXME
    float near = 0.1f;
    float far = 1000.0f;
    mat4 projection = glm::perspective<GLfloat>(fov, aspect, near, far);
};

struct Model {
    Mesh *mesh;
    vec4 color = vec4(1);
};

struct Controller {
    float speed = 1.0f;
    float mouse_sensitivity = 0.002f;
};

