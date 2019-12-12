#include "Window.h"

int Window::width;
int Window::height;

int Window::shiftFlag(1);

const char* Window::windowTitle = "GLFW Project";

// Objects to display.

int Window::mouseLeftButtonFlag(0);
glm::vec3 Window::cursorPrevPos;



Geometry *Window::tempGeo;
Sphere *Window::sphere;
Transform* Window::world, * Window::sphere2World, * Window::sky2World, * Window::track2World;
glm::mat4 Window::projection; // Projection matrix.


GLuint Window::projectionLoc; // Location of projection in shader.
GLuint Window::program; // The shader program id.

// View matrix, defined by eye, center and up.
glm::mat4 Window::view;
GLuint Window::viewLoc; // Location of view in shader.

Camera* Window::camera;
SkyBox* Window::skyBox;

int Window::cullingDemoMode(0);

GLuint Window::programSky;

BeizerTrack *Window::track;

GLint Window::modelRotateEnable(1), Window::lightRotateEnable(0);


bool Window::initializeProgram() {

	// Create a shader program with a vertex shader and a fragment shader.
	program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
	std::cerr << "shader ID " << program<< std::endl;
	// Check the shader program.
	if (!program)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}
	
	/*****************************REQUIRED**************************************/
	programSky = LoadShaders("shaders/shaderSky.vert", "shaders/shaderSky.frag");
	std::cerr << "shader ID " << programSky << std::endl;
	if (!programSky)
	{
		std::cerr << "Failed to initialize shaderSky program" << std::endl;
		return false;
	}
	/***************************************************************************/
	
	/***************************MAY DIFFERENT*********************************/
	camera = new Camera();
	camera->eye = glm::vec3(0.0f, 5.0f, 30.0f); // Camera position.
	camera->center = glm::vec3(0, 0, 0); // The point we are looking at.
	camera->up = glm::vec3(0, 1, 0); // The up direction of the camera.
	camera->FOV = 60.0;
	camera->nearP = 1.0;
	camera->farP = 1000.0;
	camera->refreshCamera();
	/*************************************************************************/

	/***************************REQUIRED**************************************/
	GLuint programParticles = LoadShaders("shaders/shaderParticle.vert", "shaders/shaderParticle.frag");
	std::cerr << "shader ID " << programParticles << std::endl;
	if (!programSky)
	{
		std::cerr << "Failed to initialize shaderParticles program" << std::endl;
		return false;
	}
	
	Particles* tempPar = new Particles();
	tempPar->setParameters(programParticles, camera);
	/***************************************************************************/
	
	
	
	

	return true;
}

bool Window::initializeObjects()
{
	glm::mat4 R, T, S;
	
	

	world = new Transform(glm::mat4(1), 0, 0, 0, camera);
	sky2World = new Transform(glm::mat4(1), 0, 0, 0, camera);

	skyBox = new SkyBox(20.0f, programSky, camera);

	world->addChild(sky2World);

	sky2World->addChild(skyBox);

	/***************REQUIRED*******************/
	Tree* newTree = new Tree(program, camera);
	Transform* oak = newTree->oak();
	Transform* bush = newTree->bush();
	Transform* vine = newTree->vine();
	T = glm::translate(glm::vec3(-8,-5.5,0));
	Transform* tree2World1 = new Transform(T, 0, 0, 0, camera);
	tree2World1->addChild(vine);
	world->addChild(tree2World1);

	T = glm::translate(glm::vec3(8, -5.5, 0));
	Transform* tree2World2 = new Transform(T, 0, 0, 0, camera);
	tree2World2->addChild(oak);
	world->addChild(tree2World2);
	/****************************************/
	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	//delete bunnyPoints, dragonPoints, bearPoints, lightObj;

	// Delete the shader program.
	glDeleteProgram(program);
	//glDeleteProgram(programLight);
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
	/*
	std::cout << "resizeCallBack" << std::endl;
	std::cout << "width = " << width;
	std::cout << ", height = " << height << std::endl;
	*/
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	/*
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(60.0), 
		double(width) / (double)height, 1.0, 1000.0);
*/
}

void Window::idleCallback()
{
	// Perform any updates as necessary. 
	//currentObj->update(); //comment: spin 9/29
	//currentRot->update();

	/*********REQUIRED**********/
	world->update();
	/************************/
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	


	// Set the viewport size.
	glViewport(0, 0, width, height);

	//robot->resetVisibilityCount();


	// Render the object.

	//std::cout << "currentRot->draw" << std::endl;
	//Army->draw(glm::mat4(1));
	camera->winWidth = width;
	camera->winHeight = height;
	//camera->refreshCamera();
	//skyBox->draw(glm::mat4(1));
	world->draw(glm::mat4(1));

	//std::string curNum = std::to_string(robot->getVisibilityCount());
	//char newTitle[20] = "Visible Robot: ";
	//strcat(newTitle, curNum.c_str());
	//const char* title = newTitle;
	//glfwSetWindowTitle(window, title);

	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Modify below to add your key callbacks.
	 */
	
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;

		case GLFW_KEY_S:
			//sphere->setSphereFlag(1);
			break;
		//case GLFW_KEY_C:
			//robot->cullingEnable();
			break;
		case GLFW_KEY_D:
			//cullingDemoMode = (cullingDemoMode + 1) % 2;
			break;
		case GLFW_KEY_LEFT:
			track->selectPoint(-1);
			break;
		case GLFW_KEY_RIGHT:
			track->selectPoint(1);
			break;
		case GLFW_KEY_X:
			track->changePosition(0, shiftFlag);
			break;
		case GLFW_KEY_Y:
			track->changePosition(1, shiftFlag);
			break;
		case GLFW_KEY_Z:
			track->changePosition(2, shiftFlag);
			break;
		case GLFW_KEY_LEFT_SHIFT:
			shiftFlag *= -1;
			break;
		case GLFW_KEY_P:
			track->moveEnable();
			break;
		case GLFW_KEY_C:
			track->riderViewEnable();
			sphere->setSphereFlag(1);
			break;
		case GLFW_KEY_V:
			track->physicsSimEnable();
			break;
		case GLFW_KEY_1:
			modelRotateEnable = (modelRotateEnable+ 1)%2;
			//lightRotateEnable = 0;
			break;

		case GLFW_KEY_2:
			//modelRotateEnable = 0;
			//lightRotateEnable = 1;
			break;

		case GLFW_KEY_3:
			//modelRotateEnable = 1;
			//lightRotateEnable = 1;
			break;

		default:
		break;
		}
	}
}

void Window::scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	//GLfloat  factor = pow(1.05, yOffset);
	/*
	if (modelRotateEnable == 1)
	{
		//currentRot->scaleModel(factor);
	}


	if (lightRotateEnable == 1)
	{
		//glm::vec3 currentPosition = lightObj->getModelCenter();
		//glm::vec3 destination = factor * currentPosition;
		//lightObj->translateModel(destination-currentPosition);
		//light.position = lightObj->getModelCenter();

	}*/
	//FOV = FOV * factor;
	camera->FOV = camera->FOV + yOffset/4;
	camera->FOV = (camera->FOV > 120) ? 120 : camera->FOV;
	camera->FOV = (camera->FOV < 10) ? 10 : camera->FOV;
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			mouseLeftButtonFlag = 1;
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			cursorPrevPos = cursorPositionMapping(xpos, ypos);
		}
		else if (action == GLFW_RELEASE)
		{
			mouseLeftButtonFlag = 0;
			
		}
	}
}

void Window::cursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
	
	if (mouseLeftButtonFlag == 1)
	{
		glm::vec3 v;
		v = cursorPositionMapping(xPos, yPos);
		
		if (modelRotateEnable == 1)
		{
			world->rotateModel(cursorPrevPos, v);
		}
		else
		{
			camera->rotateCamera(cursorPrevPos, v);
		}
		/*
		if (lightRotateEnable == 1)
		{
			//lightObj->rotateModel(cursorPrevPos, v);
			//light.position = lightObj->getModelCenter();
		}*/
		
		cursorPrevPos = v;
	}
}

glm::vec3 Window::cursorPositionMapping(double xPos, double yPos)
{
	glm::vec3 v;

	xPos = (xPos < 640.0f) ? xPos : 640.f;
	xPos = (xPos > 0.0f) ? xPos : 0.0f;
	xPos = (xPos < 480.0f) ? xPos : 480.f;
	xPos = (xPos > 0.0f) ? xPos : 0.0f;

	//v.x = (xPos - 640.0f / 2) / 400.0f; // sqrt(320^2 + 240^2)
	//v.y = (480.0f / 2 - yPos) / 400.0f;
	v.x = (xPos - 320.0f) / 320.0f;
	v.y = (240.0f - yPos) / 240.0f;

	//std::cout << "v.x = " << v.x << ", v.y = " << v.y << std::endl;

	v.z = sqrt(1.0f - v.x * v.x - v.y * v.y);

	v = glm::normalize(v);
	return v;
}
