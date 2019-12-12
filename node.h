#ifndef _NODE_H_
#define _NODE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>

#include "Player.h"

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;


	glm::vec3 specular;
	GLfloat shininess;
};

struct Light
{
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat attenuationPara;
};


class Node
{
protected:
	glm::mat4 model;
	glm::vec3 color;
	glm::vec3 modelCenter;

public:
	glm::mat4 getModel() { return model; }
	glm::vec3 getColor() { return color; }
	glm::vec3 getModelCenter() { return modelCenter; }

	virtual void draw(glm::mat4 C) = 0;
	virtual void update() = 0;
};

/*
class Camera : public Node
{
private:
	// View matrix, defined by eye, center and up.
	glm::mat4 view, projection;

	//8 points
	glm::vec3 nearCenter;
	float nearHalfHight;
	float nearHalfLength;
	glm::vec3 nearTL;
	glm::vec3 nearTR;
	glm::vec3 nearBL;
	glm::vec3 nearBR;

	glm::vec3 farCenter;
	float farHalfHight;
	float farHalfLength;
	glm::vec3 farTL;
	glm::vec3 farTR;
	glm::vec3 farBL;
	glm::vec3 farBR;

	//normal of 6 sides
	glm::vec3 nearPNorm;
	glm::vec3 farPNorm;
	glm::vec3 topPNorm;
	glm::vec3 bottomPNorm;
	glm::vec3 leftPNorm;
	glm::vec3 rightPNorm;

	double aspect;
	glm::vec3 direction;
	glm::vec3 right;

	glm::vec3 initialDir = glm::vec3(0,0,-1);
	glm::mat4 model = glm::mat4(1);

public:
	Camera();
	~Camera();

	void draw(glm::mat4 C) {}
	void update() {}

	void refreshCamera();
	bool inFrustum(glm::vec3 position, float radius);
	float point2PlaneDist(glm::vec3 point, glm::vec3 pointOnPlane, glm::vec3 normal);

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	void rotateCamera(glm::vec3 prevCursorPos, glm::vec3 curCursorPos);

	glm::vec3 eye = glm::vec3(0, 0, 20); // Camera position.
	glm::vec3 center = glm::vec3(0, 0, 0); // The point we are looking at.
	glm::vec3 up = glm::vec3(0, 1, 0); // The up direction of the camera.
	GLfloat FOV = 60.0;
	GLfloat nearP = 1.0;
	GLfloat farP = 1000.0;
	GLfloat winWidth = 640.0f;
	GLfloat winHeight = 480.0f;

};
*/
#endif

