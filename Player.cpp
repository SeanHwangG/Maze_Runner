//
//  Player.cpp
//  MAZERUNNER
//
//  Created by Gyuseung Hwang on 11/18/19.
//  Copyright © 2019 Gyuseung Hwang. All rights reserved.
//

#include "Player.h"

Player::Player(GLuint shader, std::string objFilename, glm::vec3 start_pos) : Geometry(shader, objFilename){
    pos = start_pos;
    dir = glm::vec3(1, 0, 0);
};

glm::mat4 Player::getView() {
    return glm::lookAt(pos, dir, glm::vec3(0, 1, 0));
}

void Player::walk() {
    pos = pos + dir;
    model = model * glm::translate(dir);
}

void Player::stop() {
    
}

void Player::draw() {
    glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(1,0,0)));
    Geometry::draw();
}
