#include "transform.h"

int Transform::cullingFlag(0);
int Transform::numOfVisibility(0);

Transform::Transform(glm::mat4 initial, GLuint animate, int cullingLevel, 
	float cullingRadius, Player* camera)//Node* child
{
	model = initial;
	//addChild(child);
	animateType = animate;
	Transform::cullingLevel = cullingLevel;
	Transform::cullingRadius = cullingRadius;
	Transform::camera = camera;
	modelCenter = glm::vec3(initial * glm::vec4(0, 0, 0, 1));
}

Transform::~Transform()
{
	
}

void Transform::rotateModel(glm::vec3 prevCursorPos, glm::vec3 curCursorPos)
{
	GLfloat degree;
	glm::vec3 normalVec = glm::cross(prevCursorPos, curCursorPos);
	if (glm::length(normalVec) > 0)
	{
		glm::mat4 rotateMatrix(1);
		normalVec = glm::normalize(normalVec);
		GLfloat cosdeg = glm::dot(prevCursorPos, curCursorPos);
		degree = glm::acos(cosdeg);
		//std::cout << "Cos = " << cosdeg << std::endl;
		//std::cout << "Degree = " << degree << std::endl;
		rotateMatrix = glm::rotate(rotateMatrix, degree, normalVec);
		TransM = rotateMatrix * TransM;
		modelCenter = rotateMatrix * glm::vec4(modelCenter, 1.0);
	}

}

void Transform::addChild(Node* child)
{
	children.push_back(child);
}

void Transform::draw(glm::mat4 M)
{
	/*
	if (cullingFlag == 1 && cullingLevel != 0)
	{
		glm::vec4 sphereCenter = glm::vec4(0,0,0,1);
		if (camera->inFrustum(glm::vec3(model * sphereCenter), cullingRadius))
		{
			numOfVisibility++;
		}
		else
		{
			return;
		}
	}*/
	//std::cout << "transform draw" << std::endl;
	std::list<Node*>::iterator i;
	for (i = children.begin(); i != children.end(); i++)
	{
		(*i)->draw(M * TransM * model);
	}
}

void Transform::update()
{
	glm::mat4 T_1, R, T;
	GLfloat timeValue = glfwGetTime();
	GLfloat curAngle = 0.7 * sin(3.14*timeValue);
	switch (animateType)
	{
		case 1:
			T = glm::translate(glm::vec3(0, -1, 0));
			T_1 = glm::translate(glm::vec3(0, 1, 0));
			R = glm::rotate(curAngle - prevAngle, glm::vec3(1, 0, 0));
			model = T_1 * R * T * model;
			break;
		case 2:
			T = glm::translate(glm::vec3(0, -1, 0));
			T_1 = glm::translate(glm::vec3(0, 1, 0));
			R = glm::rotate(-curAngle + prevAngle, glm::vec3(1, 0, 0));
			model = T_1 * R * T * model;
			break;
		case 3:
			T = glm::translate(glm::vec3(0, 1, 0));
			T_1 = glm::translate(glm::vec3(0, -1, 0));
			R = glm::rotate(-curAngle + prevAngle, glm::vec3(1, 0, 0));
			model = T_1 * R * T * model;
			break;
		case 4:
			T = glm::translate(glm::vec3(0, 1, 0));
			T_1 = glm::translate(glm::vec3(0, -1, 0));
			R = glm::rotate(curAngle - prevAngle, glm::vec3(1, 0, 0));
			model = T_1 * R * T * model;
			break;
		default:
			break;
	}

	prevAngle = curAngle;

	std::list<Node*>::iterator i;
	for (i = children.begin(); i != children.end(); i++)
	{
		(*i)->update();
	}
}

void Transform::translateModel(glm::vec3 path)
{
	model = glm::translate(path)* model;
	modelCenter = modelCenter + path;
}


