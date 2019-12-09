#ifndef _PLANTS_H_
#define _PLANTS_H_

#include "node.h"
#include "transform.h"
#include "geometry.h"
#include "images.h"
#include <map>
#include <stack>
#include <time.h>

struct State
{
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 dir = glm::vec3(0.0f, 1.0f, 0.0f);
};

class Plants
{
private:
	std::string start;
	std::string result;
	std::map<char, std::vector<std::string>> rules;
	//Geometry* trunk;
	Transform* tree;
	Camera* camera;
	int iterNum;
	int totalLevel;
	GLuint program;
	std::vector<State> leaves;
	GLuint trunkTexID, leafTexID;
	GLuint trunkTexLoc, leafTexLoc;
	

public:
	Plants(GLuint program, std::string start, std::map<char, std::vector<std::string>> rule, int iterNum);

	void iteration();
	Transform* construct();
	//void setTrunk(Geometry* obj) { trunk = obj; }
	void setCamera(Camera* cam) { camera = cam; }

	std::vector<State> getLeaves() { return leaves; }

	GLfloat trunkLength = 5.0, leafLength = 1.0, trunkRadius = 0.5;
	GLfloat xUnitDegree = 35, yUnitDegree = 30, zUnitDegree = 35;
	GLfloat lengthFactor = 0.75, radiusFactor = 0.65;
	void setTex(GLuint trunk, GLuint trunkLoc, GLuint leaf, GLuint leafLoc) 
	{ trunkTexID = trunk; leafTexID = leaf; trunkTexLoc = trunkLoc; leafTexLoc = leafLoc;}
};

class Tree
{
private:
	Transform* oakTree;
	Plants* newTree;
	Geometry* obj;
	Camera* camera;
	GLuint program;
public:
	Tree(GLuint program, Camera* camera);
	Transform* getTree();
	Transform* Tree::oak();
	GLuint loadTexture(std::string file, GLuint texStoreLoc);
};

class Trunk : public Node
{
protected:
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> pointsNormal;
	std::vector<glm::vec2> texCoord;
	std::vector<glm::ivec3> faces;

	glm::mat4 currentModel;

	GLuint vao, vbos[3], ebo;
	GLuint textureID, texStoreLoc;
	float radius = 0;

	glm::mat4 projection; //static 

	static GLuint program, modelLoc, viewLoc, projectionLoc;
	static Camera* camera;

	State state;

	int samplingTimes = 9;

public:
	Trunk(GLfloat upR, GLfloat lowR, GLfloat length, State state, GLuint textureID, GLuint texStoreLoc);
	Trunk() {}
	~Trunk();

	void draw(glm::mat4 model);
	void update();

	void setShaderPara(glm::mat4 model);

	void setCamera(Camera* camera);

	void initialization(GLfloat upR, GLfloat lowR, GLfloat length);
	void setParameter(GLuint programID, Camera* camera);
};

class Leaf : public Trunk
{
protected:
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> pointsNormal;
	std::vector<glm::vec2> texCoord;
	std::vector<glm::ivec3> faces;

	glm::mat4 currentModel;

	GLuint vao, vbos[3], ebo;

	GLuint textureID;
	float radius = 0;

	glm::mat4 projection; //static 

	static GLuint program, modelLoc, viewLoc, projectionLoc;
	static Camera* camera;

	State state;

	int samplingTimes = 36;

public:
	Leaf(GLfloat length, State state, GLuint textureID, GLuint texStoreLoc);
	Leaf() {}
	~Leaf();

	void draw(glm::mat4 model);
	void update();

	void setShaderPara(glm::mat4 model);

	void setCamera(Camera* camera);

	void initialization(GLfloat length);
	void setParameter(GLuint programID, Camera* camera);
};


/***********************************************************/
struct Particle {
	glm::vec3 pos, speed;
	glm::mat4 roatateMatrix;
	float weight = 1;
	float life; // Remaining life of the particle. if <0 : dead and unused.
	//float cameradistance; // *Squared* distance to the camera. if dead : -1.0f
};

class Particles : public Node
{
private:
	int maxParticles, particlesCount;
	int LastUsedParticle = 0;
	int batchSize = 10;
	GLuint textureID;
	GLfloat lastTime; //glfwGetTime();
	float deltaLife = 0.1;
	std::vector<Particle> particlesContainer;
	std::vector<State> sources;
	
	static Camera* camera;
	static GLuint programID, modelLoc, viewLoc, projectionLoc;

	GLuint vao, vbos[4], ebo;

	std::vector<glm::vec3> points;
	std::vector<glm::vec3> pointsNormal;
	std::vector<glm::vec3> centers;
	std::vector<glm::vec2> texCoord;
	std::vector<glm::ivec3> faces;

public:
	Particles() {}
	~Particles() {}
	void initial(std::vector<State> sources);
	void draw(glm::mat4 C);
	void update();
	void loadTexture(std::string file, GLuint texStoreLoc);
	int findUnusedParticle();
	
	void setParameters(GLuint programID, Camera* camera);
	void setShaderPara(glm::mat4 model);
};

#endif // _PLANTS_H_

