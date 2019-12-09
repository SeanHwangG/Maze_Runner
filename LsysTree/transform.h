#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <list>

#include<fstream>
#include<iostream>
#include<sstream>
#include<math.h>

#include "node.h"

class Transform : public Node
{
private:
	glm::mat4 TransM = glm::mat4(1);
	std::list<Node*> children;
	GLuint animateType;
	GLfloat prevAngle = 0;

	static int numOfVisibility;
	//static Camera *camera;
	static int cullingFlag;
	int cullingLevel;

	float cullingRadius;

	Camera* camera;

public:
	Transform(glm::mat4 initial, GLuint animate, int cullingLevel, 
		float cullingRadius, Camera* camera);//Node* child
	~Transform();

	void draw(glm::mat4 model);
	void update();

	void addChild(Node* child);
	void rotateModel(glm::vec3 prevCursorPos, glm::vec3 curCursorPos);

	void scaleModel(GLfloat parameter);

	void translateModel(glm::vec3 path);

	void resetVisibilityCount() { numOfVisibility = 0; }
	int getVisibilityCount() { return numOfVisibility; }
	void setCamera(Camera* curCamera) { Transform::camera = curCamera; }

	void cullingEnable() { cullingFlag = (cullingFlag + 1) % 2; }

	void setModelMatrix(glm::mat4 curModel) { model = curModel; }

};

#endif //ifndef _TRANSFORM_H_
