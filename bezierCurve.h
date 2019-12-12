#ifndef _BEIZER_CURVE_H
#define _BEIZER_CURVE_H


#include "node.h"
#include "geometry.h"
#include "transform.h"

struct movingObj
{
	Transform* obj;
	GLfloat velocity = 4.2;
	GLfloat mass = 1;
	GLfloat g = 9.8;
	GLfloat sumDis = 0;
	GLuint nextPoint = 1;
	GLint direction = -1;
	glm::vec3 position;
	GLfloat energy;
};

class BeizerCurve : public Node
{
private:
	std::vector<glm::vec3> handel1, handel2;
	glm::mat4 model;
	GLuint program, modelLoc, viewLoc, projectionLoc, cameraPosLoc;
	GLuint type, typeLoc;
	Camera* camera;

	GLuint vaos[3];
	GLuint vbos[3];

protected:
	static glm::mat4 B_bez;
	static GLuint samplingTimes;
	static Geometry* mark;

public:
	BeizerCurve(glm::vec3 point0, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3,
		GLuint programID, Camera* camera);
	~BeizerCurve();

	void draw(glm::mat4 C);
	void update();

	glm::vec3 getPosition(GLfloat t);
	glm::vec3 getTangent(GLfloat t);
	void sampling(glm::mat4 C);
	void setMatrix(glm::mat4 curModel);

	void setMark(Geometry* temp) { mark = temp; }

	GLuint markType[3] = { 4, 4, 4 };
	std::vector<glm::vec3> line;
	glm::mat4 G_bez;
};

class BeizerTrack : public Node
{
private:
	GLuint numofCurves = 8;
	std::vector<BeizerCurve*> track;
	GLint curControlPoint = 0;
	movingObj obj;
	GLuint samplingTimes = 150;
	GLuint moveFlag = 1;
	GLuint riderFlag = 0;
	Camera* camera;

	glm::vec3 tempEye, tempCenter, tempUp;

	GLuint physicsFlag = 0;
	GLfloat tempVelocity;

public:
	BeizerTrack(GLuint programID, Camera* camera);
	~BeizerTrack();
	void draw(glm::mat4 C);
	void update();

	//void addChild(BeizerCurve *curve);
	void continuity(GLuint curveIndex1, GLuint curveIndex2);
	void selectPoint(int movement);
	void changePosition(int axis, int direction);
	void setObject(Transform* object);

	void moveEnable();
	void riderViewEnable();
	void physicsSimEnable();
};

#endif
