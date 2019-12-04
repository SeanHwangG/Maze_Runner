#include "Wall.h"

Wall::Wall(GLuint shader, int i, int j, int height) : Geometry(shader, "data/box.obj")
{
    //model *= glm::translate(glm::vec3(-40 + 80 * i / (double)50, -50, -40 + 80 * j / (double)50));
    model *= glm::translate(glm::vec3(-40 + i, -50, -40 + j));
    
}

Wall::~Wall()
{
	glDeleteBuffers(2, vbos);
	glDeleteVertexArrays(1, &vao);
}

void Wall::draw() {
    Geometry::draw();
}
