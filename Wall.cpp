#include "Wall.h"

Wall::Wall(GLuint shader, int i, int j, int height, Geometry* obj)
{
    pos = glm::vec3(i, 0, j);
    model = glm::translate(pos);
    model *= glm::scale(glm::vec3(1, 1.5, 1));
    wall_object = obj;
}

Wall::~Wall()
{
    delete wall_object;
}

void Wall::draw() {
    wall_object->draw(model, glm::vec3(0.2, 0.2, 0.2));
}
