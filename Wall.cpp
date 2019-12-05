#include "Wall.h"

Wall::Wall(GLuint shader, int i, int j, int height) : Geometry(shader, "data/box.obj")
{
    model *= glm::translate(glm::vec3(i, 0, j));
    model *= glm::scale(glm::vec3(1, 1, 1));
}

Wall::~Wall()
{
	glDeleteBuffers(2, vbos);
	glDeleteVertexArrays(1, vaos);
}

void Wall::draw() {
    Geometry::draw();
}
