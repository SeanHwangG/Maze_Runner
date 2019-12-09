#include "Window.h"
#include <typeinfo>
#include <iostream>

bool Window::is_birdeye = false;

int Window::width;
int Window::height;

const char* Window::windowTitle = "GLFW Starter Project";

Player * Window::player;
Skybox * Window::skybox;
Maze * Window::maze;
glm::vec3 Window::lastPos;

glm::mat4 Window::projection; // Projection matrix.
glm::vec3 Window::eye(22, 60, 24); // Camera position.
glm::vec3 Window::center(22, 0, 24); // The point we are looking at.
glm::vec3 Window::up(0, 0, 1); // The up direction of the camera.
glm::mat4 Window::view = glm::lookAt(Window::eye, Window::center, Window::up);

GLuint Window::object_shader;
GLuint Window::skybox_shader;

// YW
glm::vec3 Window::camera_direction(glm::normalize(center - eye));
glm::vec2 Window::cursorPoint;
glm::vec3 Window::lastPoint;
bool Window::movement = false;


bool Window::initializeProgram() {
    object_shader = LoadShaders("shaders/object.vert", "shaders/object.frag");
    skybox_shader = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");

    if (!object_shader || !skybox_shader)
    {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }
        
    skybox = new Skybox(skybox_shader);
    maze = new Maze(object_shader, "data/box.obj");
    player = new Player(object_shader, "data/sphere.obj", glm::vec3(0, 1, 0), maze);
    
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
        double(width) / (double)height, 0.01, 1000.0);
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
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case GLFW_KEY_B:
            is_birdeye = !is_birdeye;
            break;
        }
    }
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_LEFT:
            player->turn_angle = 1;
            break;
        case GLFW_KEY_RIGHT:
            player->turn_angle = -1;
            break;
        case GLFW_KEY_UP:
            player->is_walking = true;
            break;
        case GLFW_KEY_DOWN:
            player->is_backing = true;
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
            case GLFW_KEY_RIGHT:
                player->turn_angle = 0;
                break;
            case GLFW_KEY_DOWN:
                player->is_backing = false;
        }
    }
    // YW
    if (action == GLFW_PRESS && is_birdeye) {
        switch (key) {
        case GLFW_KEY_W:
            eye += glm::vec3(0, 0, 1);
            center = glm::vec3(eye.x, 0, eye.z);
            view = glm::lookAt(Window::eye, Window::center, Window::up);
            break;
        case GLFW_KEY_S:
            eye += glm::vec3(0, 0, -1);
            center = glm::vec3(eye.x, 0, eye.z);
            view = glm::lookAt(Window::eye, Window::center, Window::up);
            break;
        case GLFW_KEY_A:
            eye += glm::vec3(1, 0, 0);
            center = glm::vec3(eye.x, 0, eye.z);
            view = glm::lookAt(Window::eye, Window::center, Window::up);
            break;
        case GLFW_KEY_D:
            eye += glm::vec3(-1, 0, 0);
            center = glm::vec3(eye.x, 0, eye.z);
            view = glm::lookAt(Window::eye, Window::center, Window::up);
            break;
        }
    }
    if (action == GLFW_REPEAT && is_birdeye) {
        switch (key) {
        case GLFW_KEY_W:
            eye += glm::vec3(0, 0, 1);
            center = glm::vec3(eye.x, 0, eye.z);
            view = glm::lookAt(Window::eye, Window::center, Window::up);
            break;
        case GLFW_KEY_S:
            eye += glm::vec3(0, 0, -1);
            center = glm::vec3(eye.x, 0, eye.z);
            view = glm::lookAt(Window::eye, Window::center, Window::up);
            break;
        case GLFW_KEY_A:
            eye += glm::vec3(1, 0, 0);
            center = glm::vec3(eye.x, 0, eye.z);
            view = glm::lookAt(Window::eye, Window::center, Window::up);
            break;
        case GLFW_KEY_D:
            eye += glm::vec3(-1, 0, 0);
            center = glm::vec3(eye.x, 0, eye.z);
            view = glm::lookAt(Window::eye, Window::center, Window::up);
            break;
        }
    }
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    if (is_birdeye) {
        std::cout << yoffset << std::endl;
        eye += glm::vec3(0, yoffset, 0);
        view = glm::lookAt(Window::eye, Window::center, Window::up);
    }
}


void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        movement = true;
        lastPoint = trackBallMapping(window, cursorPoint.x, cursorPoint.y);
        glMatrixMode(GL_MODELVIEW); // don't know what this is for
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        movement = false;
    }
}

void Window::cursor_callback(GLFWwindow* window, double xpos, double ypos) {
    Window::cursorPoint.x = xpos;
    Window::cursorPoint.y = ypos;
    if (movement) {
        glm::vec3 curPoint = trackBallMapping(window, xpos, ypos);
        glm::vec3 direction = curPoint - lastPoint;
        float velocity = glm::length(direction);
        if (velocity > 0.0001) {
            glm::vec3 rotAxis = glm::cross(lastPoint, curPoint);
            float rot_angle = glm::acos(glm::dot(lastPoint, curPoint) / (lastPoint.length() * curPoint.length()));
            camera_direction = glm::normalize(glm::vec3(glm::vec4(camera_direction, 1.0f) * glm::rotate(rot_angle / 20, rotAxis)));
            view = glm::lookAt(Window::eye, Window::eye + Window::camera_direction, Window::up);
            //view = glm::rotate(rot_angle / 100, rotAxis) * view;
            
        }
        lastPoint = curPoint;
    }
}

glm::vec3 Window::trackBallMapping(GLFWwindow* window, double xps, double yps) {
    glm::vec3 v;
    float d;
    v.x = (2.0f * xps - Window::width) / Window::width;
    v.y = (Window::height - 2.0f * yps) / Window::height;
    v.z = 0.0;
    d = glm::length(v);
    d = (d < 1.0) ? d : 1.0;
    v.z = sqrtf(1.001f - d * d);
    v = glm::normalize(v);
    return v;
}
