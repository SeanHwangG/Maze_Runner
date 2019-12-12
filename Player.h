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
#include "Maze.h"
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
    Maze* maz;
    bool is_walking = false;
    bool is_backing = false;
    float turn_angle = 0;
    float angle = 0;
    float radius = 0.4;
    
    Player(GLuint shader, std::string objFilename, glm::vec3 start_pos, Maze* maz);
    void draw();

};

#endif
