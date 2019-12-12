#include "Wall.h"

Wall::Wall(int i, int j, int height, Geometry* wall_obj, Geometry* wall_fm)
{
    pos = glm::vec3(i, 0, j);
    model = glm::translate(pos);
    model *= glm::scale(glm::vec3(1, 1.5, 1));
    wall_object = wall_obj;
    wall_frame = wall_fm;
}

Wall::~Wall()
{
    delete wall_object;
    delete wall_frame;
}

void Wall::draw(bool collision_on) {
    wall_object->draw(model, glm::vec3(0.2, 0.2, 0.2));
    if (collision_on) {
        glm::vec3 col;
        if (collided) {
            col = glm::vec3(1, 0, 0);
            //std::cout << "draw red on " << pos.x << " " << pos.z << std::endl;
        }
        else
            col = glm::vec3(0, 1, 0);
        wall_frame->drawlines(model * glm::translate(glm::vec3(-0.1, 0, -0.1)) * glm::scale(glm::vec3(1.2, 1.01, 1.2)), col);
    }
}
