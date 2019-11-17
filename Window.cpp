#include "Window.h"
#include <typeinfo>
#include <iostream>

int Window::width;
int Window::height;

const char* Window::windowTitle = "GLFW Starter Project";

Cube * Window::cube;
Geometry * Window::player;
Skybox * Window::skybox;

glm::mat4 Window::projection; // Projection matrix.

glm::vec3 Window::eye(0, 0, 20); // Camera position.
glm::vec3 Window::center(0, 0, 0); // The point we are looking at.
glm::vec3 Window::up(0, 1, 0); // The up direction of the camera.
glm::mat4 Window::view = glm::lookAt(Window::eye, Window::center, Window::up);

GLuint Window::object_shader;
GLuint Window::skybox_shader;

bool Window::initializeProgram() {
    object_shader = LoadShaders("shaders/object.vert", "shaders/object.frag");
    skybox_shader = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");
    
    if (!object_shader || !skybox_shader)
    {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }
    
    player = new Geometry("objects/sphere.obj", object_shader);
    skybox = new Skybox(skybox_shader);
    
    return true;
}

void Window::cleanUp()
{
    delete player;
    glDeleteProgram(skybox_shader);
    glDeleteProgram(object_shader);
}

GLFWwindow* Window::createWindow(int width, int height)
{
    // Initialize GLFW.
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return NULL;
    }

    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    // Apple implements its own version of OpenGL and requires special treatments
    // to make it uses modern OpenGL.

    // Ensure that minimum OpenGL version is 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Enable forward compatibility and allow a modern OpenGL context
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create the GLFW window.
    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

    // Check if the window could not be created.
    if (!window)
    {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return NULL;
    }

    // Make the context of the window.
    glfwMakeContextCurrent(window);

#ifndef __APPLE__
    // On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

    // Initialize GLEW.
    if (glewInit())
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return NULL;
    }
#endif

    // Set swap interval to 1.
    glfwSwapInterval(0);

    // Call the resize callback to make sure things get drawn immediately.
    Window::resizeCallback(window, width, height);

    return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
    // In case your Mac has a retina display.
    glfwGetFramebufferSize(window, &width, &height);
#endif
    Window::width = width;
    Window::height = height;
    // Set the viewport size.
    glViewport(0, 0, width, height);

    // Set the projection matrix.
    Window::projection = glm::perspective(glm::radians(60.0),
        double(width) / (double)height, 1.0, 1000.0);
}

void Window::idleCallback()
{
}

void Window::displayCallback(GLFWwindow* window)
{
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(object_shader);
    glUniformMatrix4fv(glGetUniformLocation(object_shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(object_shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(object_shader, "model"), 1, GL_FALSE, glm::value_ptr(player->model));
    player->draw();
    
    glUseProgram(skybox_shader);
    glUniformMatrix4fv(glGetUniformLocation(skybox_shader, "projection"), 1, GL_FALSE,
                       glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(skybox_shader, "view"), 1, GL_FALSE,
                       glm::value_ptr(view));
    skybox->draw();

    glfwPollEvents();
    glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_ESCAPE:
            // Close the window. This causes the program to also terminate.
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case GLFW_KEY_LEFT:
            break;
        case GLFW_KEY_RIGHT:
            break;
        case GLFW_KEY_UP:
            break;
        case GLFW_KEY_DOWN:
            break;
        default:
            break;
        }
    }
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
}
