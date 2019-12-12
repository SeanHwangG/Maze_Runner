#ifndef _WINDOW_H_
#define _WINDOW_H_

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

#include "node.h"
#include "transform.h"
#include "geometry.h"
//#include "PointCloud.h"
#include "shader.h"
#include "SkyBox.h"
#include "bezierCurve.h"

#include "Plants.h"

class Window
{
public:
	static int width;
	static int height;
	static const char* windowTitle;

	//static PointCloud* currentObj;
	static Transform* currentRot;

	static glm::mat4 projection;
	static glm::mat4 view;
	static GLuint program, viewLoc, projectionLoc;// ,  modelLoc;// , colorLoc;
	static GLuint programSky;
	//static GLuint  viewPosLoc;
	//static GLuint materialAmbientLoc, materialDiffuseLoc, materialSpecularLoc, materialShininessLoc;
	//static GLuint lightPositionLoc, lightAmbientLoc, lightDiffuseLoc, lightSpecularLoc, lightAttenuationParaLoc;
	//static GLint colorTypeLoc,colorType;
	static GLint modelRotateEnable, lightRotateEnable;

	//static glm::vec3 eye, center, up;
	//static float FOV, nearP, farP;
	static Camera *camera;
	static SkyBox *skyBox;

	//Robot
	//static PointCloud* Window::bunnyPoints;
	//static PointCloud* Window::dragonPoints;
	//static PointCloud* Window::bearPoints;
	//static Transform *Army, *robot, *leftLeg2Robot, *rightLeg2Robot, *leftArm2Robot, *rightArm2Robot;
	//static Transform *head2Robot, *leftEye2Head, *rightEye2Head, *leftAntenna2Head, *rightAntenna2Head;
	static Geometry *tempGeo;
	static Sphere* sphere;
	static Transform *world, *sphere2World, *sky2World, *track2World;

	//static float winLength;
	static int shiftFlag;
	static glm::vec3 cursorPrevPos;

	static int mouseLeftButtonFlag;

	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void idleCallback();
	static void displayCallback(GLFWwindow*);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
	static glm::vec3 Window::cursorPositionMapping(double xPos, double yPos);

	static int cullingDemoMode;
	//static Material material;
	//static Light light;

	//LightObj
	//static PointCloud* lightObj;
	//static GLuint programLight, projectionLocLight, viewLocLight, modelLocLight, colorLocLight;

	static BeizerTrack* track;
};

#endif