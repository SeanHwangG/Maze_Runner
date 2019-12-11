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
#define DIM 64
#define RANGE 0.04
#define CHANGE 0.00005
#define SEED (time(0))
using namespace std;
int tx[4] = { 0, 0, 1, -1 };
int ty[4] = { 1, -1, 0, 0 };
vector<glm::vec3> vertices;
vector<glm::vec3> vertexNormal;
vector<glm::ivec3> faces;

struct poi {
	glm::vec3 point;
	int x, y;
};

float random_float(float a, float b) {
	srand(SEED);
	return ((b - a) * ((float)rand() / RAND_MAX)) + a;
}

poi getMid(poi& p1, poi& p2, float range) {
	int midx = (p1.x + p2.x) / 2;
	int midy = (p1.y + p2.y) / 2;
	glm::vec3 midpoint = (p1.point + p2.point) / 2.0f;
	midpoint.z = random_float(midpoint.z - range, midpoint.z + range);
	poi midpoi = { midpoint, midx, midy };
	return midpoi;
}

// generate terrain point data for a single face, assuming they have the same z coordinate
vector<vector<glm::vec3>> MidpointDisplacement(vector<glm::vec3> single, int mode) {
	poi field[DIM][DIM];
	for (int i = 0; i != DIM; i++)
		for (int j = 0; j != DIM; j++)
			field[i][j] = { glm::vec3(0, 0, 0), -1, -1 };
	poi v[4] = { { single[0], 0, 0 }, { single[1], 0, DIM - 1 }, { single[2], DIM - 1, DIM - 1 }, { single[3], DIM - 1, 0 } };
	for (int i = 0; i != 4; i++)
		field[v[i].x][v[i].y] = v[i];
	float rang = RANGE;
	queue<poi> que;
	for (int i = 0; i != 4; i++)
		que.push(v[i]);
	int count = 0;
	while (!que.empty()) {
		poi now = que.front();
		for (int t = 0; t != 4; t++) {
			int nx = now.x + tx[t];
			int ny = now.y + ty[t];
			if (nx < 0 || ny < 0 || nx >= DIM || ny >= DIM) continue;
			if (field[nx][ny].x != -1) continue;
			while (field[nx][ny].x == -1) {
				nx += tx[t];
				ny += ty[t];
			}
			if (nx < 0 || ny < 0 || nx >= DIM || ny >= DIM) continue;
			poi mid = getMid(now, field[nx][ny], rang);
			if (mid.x == 0 || mid.x == DIM - 1 || mid.y == 0 || mid.y == DIM - 1)
				mid.point.z = now.point.z;
			rang -= CHANGE;
			if (rang < 0)
				rang = 0;
			field[mid.x][mid.y] = mid;
			que.push(mid);
			/* test 
			system("cls");
			for (int i = 0; i != DIM; i++) {
				for (int j = 0; j != DIM; j++) {
					if (field[i][j].x != -1)
						cout << "1 ";
					else
						cout << "  ";
				}
				cout << endl;
			}
			system("pause");
			*/
		}
		que.pop();
		count++;
	}

	vector<vector<glm::vec3>> fmap;
	for (int i = 0; i != DIM; i++) {
		fmap.push_back(vector<glm::vec3>());
		for (int j = 0; j != DIM; j++) {
			switch (mode) {
			case 0:
				break;
			case 1:
				swap(field[i][j].point.x, field[i][j].point.z);
				break;
			case 2:
				swap(field[i][j].point.y, field[i][j].point.z);
				break;
			}
			fmap[i].push_back(field[i][j].point);
			vertices.push_back(field[i][j].point);
		}
	}
	return fmap;
}

inline int getNumber(int i, int j) {
	return i * DIM + j;
}

inline glm::vec3 getFaceNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
	return glm::normalize(glm::cross(b - a, c - a));
}

void computeFaces(vector<vector<glm::vec3>>& vertex) {
	int base = vertexNormal.size();
	vector<vector<glm::vec3>> faceNormals;
	for (int i = 0; i != DIM * DIM; i++)
		faceNormals.push_back(vector<glm::vec3>());
	for (int i = 0; i != DIM - 1; i++) {
		for (int j = 0; j != DIM - 1; j++) {
			glm::vec3 v1 = vertex[i][j];
			unsigned num1 = getNumber(i, j);
			glm::vec3 v2 = vertex[i][j + 1];
			unsigned num2 = getNumber(i, j+1);
			glm::vec3 v3 = vertex[i + 1][j + 1];
			unsigned num3 = getNumber(i+1, j+1);
			glm::vec3 v4 = vertex[i + 1][j];
			unsigned num4 = getNumber(i+1, j);

			glm::vec3 tri1 = getFaceNormal(v1, v3, v2);
			faceNormals[num1].push_back(tri1);
			faceNormals[num3].push_back(tri1);
			faceNormals[num2].push_back(tri1);
			faces.push_back(glm::ivec3(num1 + base + 1, num3 + base + 1, num2 + base + 1));

			glm::vec3 tri2 = getFaceNormal(v1, v4, v3);
			faceNormals[num1].push_back(tri2);
			faceNormals[num4].push_back(tri2);
			faceNormals[num3].push_back(tri2);
			faces.push_back(glm::ivec3(num1 + base + 1, num4 + base + 1, num3 + base + 1));
		}
	}
	for (auto vec : faceNormals) {
		glm::vec3 sum(0, 0, 0);
		for (auto normal : vec)
			sum += normal;
		vertexNormal.push_back(glm::normalize(sum));
	}
}

int main() {
	vector<glm::vec3> single{ glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 1, 0), glm::vec3(0, 1, 0) };
	auto field = MidpointDisplacement(single, 0);
	computeFaces(field);
	single = vector<glm::vec3>{ glm::vec3(0, 0, 1), glm::vec3(0, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 0, 1) };
	field = MidpointDisplacement(single, 0);
	computeFaces(field);
	single = vector<glm::vec3>{ glm::vec3(0, 0, 1), glm::vec3(1, 0, 1), glm::vec3(1, 1, 1), glm::vec3(0, 1, 1) };
	field = MidpointDisplacement(single, 1);
	computeFaces(field);
	single = vector<glm::vec3>{ glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec3(1, 1, 0), glm::vec3(1, 0, 0) };
	field = MidpointDisplacement(single, 1);
	computeFaces(field);
	single = vector<glm::vec3>{ glm::vec3(0, 0, 1), glm::vec3(1, 0, 1), glm::vec3(1, 1, 1), glm::vec3(0, 1, 1) };
	field = MidpointDisplacement(single, 2);
	computeFaces(field);
	single = vector<glm::vec3>{ glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec3(1, 1, 0), glm::vec3(1, 0, 0) };
	field = MidpointDisplacement(single, 2);
	computeFaces(field);
	ofstream out("terrain.obj");
	for (auto v : vertices)
		out << "v " << v.x << " " << v.y << " " << v.z << "\n";
	for (auto vn : vertexNormal)
		out << "vn " << vn.x << " " << vn.y << " " << vn.z << "\n";
	for (auto f : faces)
		out << "f " << f.x << "//" << f.x << " " << f.y << "//" << f.y << " " << f.z << "//" << f.z << "\n";
}
