#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm.h"
#include "assets.h"

struct Body {
    vec3 position = vec3(0);
    vec3 scale = vec3(1);
    vec3 velocity = vec3(0);
    vec3 rotation = vec3(0);
    mat4 transform = get_transform(position, scale, rotation);
};

struct Window {
    const char* title;
    int width = 1280;
    int height = 800;
    bool fullscreen = false;
    bool resizable = false;
    GLFWwindow* gl_window = nullptr;
};

struct Clock {
    float min = 0.016;
    float start_time = glfwGetTime();
    float time = 0;
    float dt = 0;
    bool running = true;
};

struct Light {
    vec3 intensity;
    //TODO add attenuation and ambient coefficient
};

struct Camera {
    float aspect = 16.0f/10.0f;
    float fov = 45;
    float near = 0.1f;
    float far = 1000.0f;
    mat4 projection = glm::perspective<GLfloat>(fov, aspect, near, far);
};

struct Model {
    MeshDef *def;
    std::string texture_file;
    Mesh mesh = {0,0,0};
    GLuint texture = 0;
};

struct Controller {
    float speed = 1.0f;
    float mouse_sensitivity = 0.002f;
};

