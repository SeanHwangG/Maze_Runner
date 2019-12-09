#include "main.h"

void error_callback(int error, const char* description)
{
	std::cerr << description << std::endl;
}

void setup_callbacks(GLFWwindow* window)
{
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, Window::keyCallback);
    glfwSetScrollCallback(window, Window::scrollCallback);
	glfwSetWindowSizeCallback(window, Window::resizeCallback);

	//YW
	// set the mouse left button down callback
	glfwSetMouseButtonCallback(window, Window::mouse_callback);
	// set the mouse cursor callback
	glfwSetCursorPosCallback(window, Window::cursor_callback);
}


void setup_opengl_settings()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void print_versions()
{
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version supported: " << glGetString(GL_VERSION) 
		<< std::endl;

#ifdef GL_SHADING_LANGUAGE_VERSION
	std::cout << "Supported GLSL version is: " << 
		glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif
}

int main(void)
{
	GLFWwindow* window = Window::createWindow(640, 480);
	if (!window) exit(EXIT_FAILURE);

	print_versions();
	setup_callbacks(window);
	setup_opengl_settings();

    if (!Window::initializeProgram()) exit(EXIT_FAILURE);
    
	while (!glfwWindowShouldClose(window))
	{
		Window::displayCallback(window);
		Window::idleCallback();
	}

	Window::cleanUp();
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}
