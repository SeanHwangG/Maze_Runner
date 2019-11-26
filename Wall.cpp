#include "Wall.h"

Wall::Wall(GLuint shader, int i, int j) : Geometry(shader, "objects/box.obj")
{
    model *= glm::translate(glm::vec3(i, 0, j));
}

Wall::~Wall()
{
	glDeleteBuffers(2, vbos);
	glDeleteVertexArrays(1, &vao);
}

void Wall::draw() {
    Geometry::draw();
}
