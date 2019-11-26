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
#include "Wall.h"

class Maze : Geometry
{
public:
    Maze(GLuint shader, std::string file_name);
    std::vector<Wall*> walls;

    void draw();
};

#endif
