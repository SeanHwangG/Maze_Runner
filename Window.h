#ifndef _WINDOW_H_
#define _WINDOW_H_
#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <memory>

#include "Object.h"
#include "Cube.h"
#include "Geometry.h"
#include "Shader.h"
#include "Skybox.h"

class Window
{
public:
	static int width;
	static int height;
	static const char* windowTitle;
	static Cube* cube;
	static Geometry * player;
    static Skybox * skybox;
	
    static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 eye, center, up;
    
    static GLuint object_shader;
    static GLuint skybox_shader;
    
	static bool initializeProgram();
	static void cleanUp();
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void idleCallback();
	static void displayCallback(GLFWwindow*);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif
