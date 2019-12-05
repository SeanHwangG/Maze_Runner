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
    glm::vec3 eye;        // Position of player
    glm::mat4 getView();
    bool is_walking = false;
    float turn_angle = 0;
    float angle = 0;
    
    Player(GLuint shader, std::string objFilename, glm::vec3 start_pos);
    void draw();
};

#endif
