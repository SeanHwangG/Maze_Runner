#include "Window.h"
#include <typeinfo>
#include <iostream>

//using namespace irrklang;


bool Window::is_birdeye = false;

int Window::width;
int Window::height;

const char* Window::windowTitle = "GLFW Starter Project";

Player * Window::player;
Skybox * Window::skybox;
Maze * Window::maze;
glm::vec3 Window::lastPos;

glm::mat4 Window::projection; // Projection matrix.
glm::vec3 Window::eye(15, 49, 15); // Camera position.
glm::vec3 Window::center(15, -40, 15); // The point we are looking at.
glm::vec3 Window::up(0, 0, -1); // The up direction of the camera.
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
    
    skybox = new Skybox(skybox_shader);
    player = new Player(object_shader, "data/sphere.obj", glm::vec3(0, -40, 0));
    maze = new Maze(object_shader, "data/maze1.txt");
    
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

    glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create the GLFW window.
    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

    if (!window)
    {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);

#ifndef __APPLE__
    // On Windows and Linux, we need GLEW to provide modern OpenGL functionality.
    if (glewInit())
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return NULL;
    }
#endif

    glfwSwapInterval(0);

    Window::resizeCallback(window, width, height);

    return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
    glfwGetFramebufferSize(window, &width, &height);
#endif
    Window::width = width;
    Window::height = height;
    
    glViewport(0, 0, width, height);

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

    glUseProgram(skybox_shader);
    glUniformMatrix4fv(glGetUniformLocation(skybox_shader, "projection"), 1, GL_FALSE,
                       glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(skybox_shader, "view"), 1, GL_FALSE,
                       glm::value_ptr(is_birdeye? view: player->getView()));
    skybox->draw();

    glUseProgram(object_shader);
    glUniformMatrix4fv(glGetUniformLocation(object_shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(object_shader, "view"), 1, GL_FALSE, glm::value_ptr(is_birdeye? view: player->getView()));
    
    player->draw();
    maze->draw();

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
        case GLFW_KEY_B:
            is_birdeye = !is_birdeye;
            break;
        case GLFW_KEY_LEFT:
            player->is_turning = -1;
            break;
        case GLFW_KEY_RIGHT:
            player->is_turning = 1;
            break;
        case GLFW_KEY_UP:
            player->is_walking = true;
            break;
        case GLFW_KEY_DOWN:
            break;
        default:
            break;
        }
    }
    if (action == GLFW_RELEASE) {
        switch (key)
        {
            case GLFW_KEY_UP:
                player->is_walking = false;
                break;
            case GLFW_KEY_LEFT:
                player->is_turning = 0;
        }

    }
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
}

glm::vec3 Window::trackBallMapping(double xpos, double ypos) {
    glm::vec3 v;
    
    v.x = (2.0 * xpos - Window::width) / (float)Window::width;
    v.y = (Window::height - 2.0 * ypos) / (float)Window::height;
    float d = (glm::length(v) < 1.0? glm::length(v): 1.0);
    v.z = sqrtf(1.001 - d*d);
    
    return -glm::normalize(v);
}

void Window::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    glm::vec3 curPos = trackBallMapping(xpos, ypos);
    if (state == GLFW_PRESS && Window::lastPos.x != 0) {
        glm::vec3 rotAxis = glm::cross(Window::lastPos, curPos);
        glm::mat4 rot = glm::rotate(glm::radians(0.7f), rotAxis);
        //player->dir = rot * glm::vec4(player->dir, 0);
        //player->center = glm::vec4(player->eye, 1) + rot * glm::vec4(player->center - player->eye, 1);
    }
    
    Window::lastPos = curPos;
}
