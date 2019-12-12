#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>

#include<fstream>
#include<iostream>
#include<sstream>
#include<math.h>

#include "node.h"

class Geometry : public Node
{
protected:
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> pointsNormal;
	std::vector<glm::ivec3> faces;

	glm::vec3 maxCoordinate;
	glm::vec3 minCoordinate;

	glm::mat4 currentModel;

	GLuint vao, vbos[2], ebo;
	float radius = 0;

	glm::mat4 projection; //static 
	
	GLuint program, modelLoc, viewLoc, projectionLoc; //static 

	Camera* camera;

public:
	Geometry(std::string objFilename, GLuint programID, Camera* camera);
	Geometry();
	~Geometry();

	void draw(glm::mat4 model);
	void update();

	void readObjFile(std::string fileName);
	void setMatrix(glm::mat4 model);

	void setCamera(Camera* camera);

	void initialization(std::string objFilename);
};

class Sphere : public Geometry
{
	
private:
	glm::mat4 projection; //static 
	GLuint cameraPosLoc, typeLoc; //program, modelLoc, viewLoc, projectionLoc
	GLuint type = 1;//static 
	GLuint sphereFlag = 1; //static 

	GLuint textureID;

public:
	Sphere(std::string objFilename, GLuint programID, Camera* camera);
	void readObjFile(std::string fileName);
	void draw(glm::mat4 model);
	void update() {}
	void setSphereFlag(GLuint flag);
	void setMatrix(glm::mat4 curModel);
	void setTextureID(GLuint ID) { textureID = ID; }
};

#endif // _GEOMETRY_H_
