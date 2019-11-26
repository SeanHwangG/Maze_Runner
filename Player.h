//
//  Player.hpp
//  MAZERUNNER
//
//  Created by Gyuseung Hwang on 11/18/19.
//  Copyright © 2019 Gyuseung Hwang. All rights reserved.
//

#ifndef Player_h
#define Player_h

#include <stdio.h>
#include "Geometry.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Player : public Geometry
{
public:
    glm::mat4 projection; // Projection matrix.
    glm::vec3 pos;
    glm::vec3 dir;
    Player(GLuint shader, std::string objFilename, glm::vec3 start_pos);
    glm::mat4 getView();
    void walk();
    void stop();
    void draw();
};

#endif
