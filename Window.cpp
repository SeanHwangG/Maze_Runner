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
GLuint Window::wall_shader;

GLuint Window::plants_shader;
GLuint Window::particle_shader;

Transform* Window::world;
Tree* Window::trees;

// YW
glm::vec3 Window::camera_direction(glm::normalize(center - eye));
glm::vec2 Window::cursorPoint;
glm::vec3 Window::lastPoint;
bool Window::movement = false;

bool Window::initializeProgram() {
    object_shader = LoadShaders("shaders/object.vert", "shaders/object.frag");
    wall_shader = LoadShaders("shaders/wall.vert", "shaders/wall.frag");
    skybox_shader = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");

    if (!object_shader || !skybox_shader || !wall_shader)
    {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }
        
    skybox = new Skybox(skybox_shader);
    maze = new Maze(object_shader, wall_shader, "data/box.obj");
    player = new Player(object_shader, "data/sphere.obj", glm::vec3(0, 1, 0), maze);


	// Create a shader program with a vertex shader and a fragment shader.
	plants_shader = LoadShaders("shaders/shaderPlants.vert", "shaders/shaderPlants.frag");
	//std::cerr << "shader ID " << program << std::endl;
	// Check the shader program.
	if (!plants_shader)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	particle_shader = LoadShaders("shaders/shaderParticle.vert", "shaders/shaderParticle.frag");
	std::cerr << "shader ID " << particle_shader << std::endl;
	if (!particle_shader)
	{
		std::cerr << "Failed to initialize shaderParticles program" << std::endl;
		return false;
	}

	Particles* tempPar = new Particles();
	tempPar->setParameters(particle_shader, player);

	world = new Transform(glm::mat4(1), 0, 0, 0, player);
	trees = new Tree(plants_shader, player);
	Transform* oak = trees->oak();
	Transform* bush = trees->bush();
	Transform* vine = trees->vine();
	glm::mat4 T = glm::translate(glm::vec3(0, 0, 0));
	glm::mat4 S = glm::scale(glm::vec3(0.5, 0.5, 0.5));
	Transform* tree2World1 = new Transform(T*S, 0, 0, 0, player);
	tree2World1->addChild(oak);
	world->addChild(tree2World1);

	//T = glm::translate(glm::vec3(8, -5.5, 0));
	//Transform* tree2World2 = new Transform(T, 0, 0, 0, player);
	//tree2World2->addChild(oak);
	//world->addChild(tree2World2);

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
	world->update();
}

void Window::displayCallback(GLFWwindow* window)
{
	player->projection = Window::projection;

    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    glUseProgram(skybox_shader);
    glUniformMatrix4fv(glGetUniformLocation(skybox_shader, "projection"), 1, GL_FALSE,
                      glm::value_ptr(player->projection));
    glUniformMatrix4fv(glGetUniformLocation(skybox_shader, "view"), 1, GL_FALSE,
                      glm::value_ptr(is_birdeye? view: player->getView()));
    skybox->draw();

    glUseProgram(object_shader);
    glUniformMatrix4fv(glGetUniformLocation(object_shader, "projection"), 1, GL_FALSE, glm::value_ptr(player->projection));
    glUniformMatrix4fv(glGetUniformLocation(object_shader, "view"), 1, GL_FALSE, glm::value_ptr(is_birdeye? view: player->getView()));
    
    player->draw();

    glUseProgram(wall_shader);
    glUniformMatrix4fv(glGetUniformLocation(wall_shader, "projection"), 1, GL_FALSE, glm::value_ptr(player->projection));
    glUniformMatrix4fv(glGetUniformLocation(wall_shader, "view"), 1, GL_FALSE, glm::value_ptr(is_birdeye ? view : player->getView()));
    glUniform3fv(glGetUniformLocation(wall_shader, "eye"), 1, glm::value_ptr(player->eye));
    maze->draw();

	world->draw(glm::mat4(1));

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
        case GLFW_KEY_C:
            maze->collision_on = !maze->collision_on;
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