//
//  Maze.hpp
//  MAZERUNNER
//
//  Created by Gyuseung Hwang on 11/18/19.
//  Copyright © 2019 Gyuseung Hwang. All rights reserved.
//

#ifndef Maze_h
#define Maze_h

#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <time.h>
#include <random>
#include <algorithm>
#include <set>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Wall.h"

/**
  Maze has list of wall pointers. Also it draws a big box which can be thought of ground.
 */
class Maze : Geometry
{
public:
    Maze(GLuint shader, GLuint wallshader, std::string file_name);
    std::vector<Wall*> walls;
    bool collision_on = true;

    void draw();
    bool collision(glm::vec3 position, float radius);
};

#endif
