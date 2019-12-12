#include "TerrainGen.h"
typedef TerrainGen::poi poi;

TerrainGen::TerrainGen(int mode, int dim) {
	dimension = dim;
	if (mode == 0)
		computeWalls();
	else
		computeSurface();
}

float TerrainGen::random_float(float a, float b) {
	return ((b - a) * ((float)rand() / RAND_MAX)) + a;
}

poi TerrainGen::getMid(poi& p1, poi& p2, float range) {
	int midx = (p1.x + p2.x) / 2;
	int midy = (p1.y + p2.y) / 2;
	glm::vec3 midpoint = (p1.point + p2.point) / 2.0f;
	midpoint.z = random_float(midpoint.z - range, midpoint.z + range);
	poi midpoi = { midpoint, midx, midy };
	return midpoi;
}
vector<vector<glm::vec3>> TerrainGen::MidpointDisplacement(vector<glm::vec3> single, int mode) {
	int tx[4] = { 0, 0, 1, -1 };
	int ty[4] = { 1, -1, 0, 0 };
	poi field[HIGHDIM][HIGHDIM];
	for (int i = 0; i != dimension; i++)
		for (int j = 0; j != dimension; j++)
			field[i][j] = { glm::vec3(0, 0, 0), -1, -1 };
	poi v[4] = { { single[0], 0, 0 }, { single[1], 0, dimension - 1 }, { single[2], dimension - 1, dimension - 1 }, { single[3], dimension - 1, 0 } };
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
			if (nx < 0 || ny < 0 || nx >= dimension || ny >= dimension) continue;
			if (field[nx][ny].x != -1) continue;
			while (field[nx][ny].x == -1) {
				nx += tx[t];
				ny += ty[t];
			}
			if (nx < 0 || ny < 0 || nx >= dimension || ny >= dimension) continue;
			poi mid;
			if (count < 20)
				mid = getMid(now, field[nx][ny], 0.07);
			else
				mid = getMid(now, field[nx][ny], rang);

			if (mid.x == 0 || mid.x == dimension - 1 || mid.y == 0 || mid.y == dimension - 1)
				mid.point.z = now.point.z;

			rang -= CHANGE;
			if (rang < 0)
				rang = 0;
			field[mid.x][mid.y] = mid;
			que.push(mid);
		}
		que.pop();
		count++;
	}
	vector<vector<glm::vec3>> fmap;
	for (int i = 0; i != dimension; i++) {
		fmap.push_back(vector<glm::vec3>());
		for (int j = 0; j != dimension; j++) {
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
			this->vertices.push_back(field[i][j].point);
		}
	}
	return fmap;
}

void TerrainGen::computeFaces(vector<vector<glm::vec3>>& vertex) {
	int base = vertexNormal.size();
	vector<vector<glm::vec3>> faceNormals;
	for (int i = 0; i != dimension * dimension; i++)
		faceNormals.push_back(vector<glm::vec3>());
	for (int i = 0; i != dimension - 1; i++) {
		for (int j = 0; j != dimension - 1; j++) {
			glm::vec3 v1 = vertex[i][j];
			unsigned num1 = getNumber(i, j);
			glm::vec3 v2 = vertex[i][j + 1];
			unsigned num2 = getNumber(i, j + 1);
			glm::vec3 v3 = vertex[i + 1][j + 1];
			unsigned num3 = getNumber(i + 1, j + 1);
			glm::vec3 v4 = vertex[i + 1][j];
			unsigned num4 = getNumber(i + 1, j);

			glm::vec3 tri1 = getFaceNormal(v1, v3, v2);
			faceNormals[num1].push_back(tri1);
			faceNormals[num3].push_back(tri1);
			faceNormals[num2].push_back(tri1);
			faces.push_back(glm::ivec3(num1 + base, num3 + base , num2 + base ));

			glm::vec3 tri2 = getFaceNormal(v1, v4, v3);
			faceNormals[num1].push_back(tri2);
			faceNormals[num4].push_back(tri2);
			faceNormals[num3].push_back(tri2);
			faces.push_back(glm::ivec3(num1 + base , num4 + base , num3 + base ));
		}
	}
	for (auto vec : faceNormals) {
		glm::vec3 sum(0, 0, 0);
		for (auto normal : vec)
			sum += normal;
		vertexNormal.push_back(glm::normalize(sum));
	}
}

void TerrainGen::computeWalls() {
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
}

void TerrainGen::computeSurface() {
	auto single = vector<glm::vec3>{ glm::vec3(0, 0, 1), glm::vec3(1, 0, 1), glm::vec3(1, 1, 1), glm::vec3(0, 1, 1) };
	auto field = MidpointDisplacement(single, 2);
	computeFaces(field);
}


void TerrainGen::generateObj() {
	ofstream out("terrain.obj");
	for (auto v : vertices)
		out << "v " << v.x << " " << v.y << " " << v.z << "\n";
	for (auto vn : vertexNormal)
		out << "vn " << vn.x << " " << vn.y << " " << vn.z << "\n";
	for (auto f : faces)
		out << "f " << f.x << "//" << f.x << " " << f.y << "//" << f.y << " " << f.z << "//" << f.z << "\n";
}
