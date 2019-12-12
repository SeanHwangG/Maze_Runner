#ifndef __TERRAINGEN__
#define __TERRAINGEN__
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>
#include<random>
#include<cmath>
#include<time.h>
#include<queue>
#include<fstream>
#define HIGHDIM 128
#define RANGE 0.05
#define CHANGE 0.0001
#define SEED (time(0))

using namespace std;

class TerrainGen {
public:
	struct poi { glm::vec3 point; int x, y; };

	vector<glm::vec3> vertices;
	vector<glm::vec3> vertexNormal;
	vector<glm::ivec3> faces;
	int dimension;

	static float random_float(float a, float b);
	static poi getMid(poi& p1, poi& p2, float range);

	TerrainGen(int mode, int dim);
	vector<vector<glm::vec3>> MidpointDisplacement(vector<glm::vec3> single, int mode);
	void computeFaces(vector<vector<glm::vec3>>& vertex);
	void computeSurface();
	void computeWalls();
	void generateObj();

	inline int getNumber(int i, int j) {
		return i * dimension + j;
	}
	inline glm::vec3 getFaceNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
		return glm::normalize(glm::cross(b - a, c - a));
	}
};
#endif
