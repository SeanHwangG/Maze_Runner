#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>

/* Modify 9/29 */
#include<fstream>
#include<iostream>
#include<sstream>
#include<math.h>

#include "Object.h"

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

class PointCloud : public Object
{
private:
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> pointsNormal;
	std::vector<glm::ivec3> faces;
	std::vector<glm::vec3> pointsColor;
	glm::vec3 modelCenter;

	GLuint vao, vbos[2], ebo;
	GLfloat pointSize;

	float radius = 0;// Modify 9/30

public:
	PointCloud(std::string objFilename, GLfloat pointSize);
	~PointCloud();

	void draw();
	void update();
	void rotateModel(glm::vec3 prevCursorPos, glm::vec3 curCursorPos);

	void updatePointSize(GLfloat size);
	void spin(float deg);

	void scaleModel(GLfloat parameter);
	
	void readObjFile(std::string fileName); // Modify 9/29
	glm::vec3 maxCoordinate;// Modify 9/30
	glm::vec3 minCoordinate;// Modify 9/30
	
	glm::vec3 getModelCenter() { return modelCenter; }
	void translateModel(glm::vec3 path);
	
};

#endif

