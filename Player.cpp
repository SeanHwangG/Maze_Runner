//
//  Player.cpp
//  MAZERUNNER
//
//  Created by Gyuseung Hwang on 11/18/19.
//  Copyright © 2019 Gyuseung Hwang. All rights reserved.
//

#include "Player.h"

Player::Player(GLuint shader, std::string objFilename, glm::vec3 start_pos) : Geometry(shader, objFilename){
    eye = start_pos;
    center = glm::vec3(1, 0, 0);
};

glm::mat4 Player::getView() {
    return glm::lookAt(eye, eye + glm::vec3(center.x, 0, center.z), glm::vec3(0, 1, 0));
}


void Player::draw() {
    if (is_walking) {
        eye = eye + center * glm::vec3(0.002, 0, 0.002);
        model = model * glm::translate(center);
    }
    if (is_turning) {
        //model = model * glm::rotate(glm::mat4(1, ));
    }

    //glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(pos));
    glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(1,0,0)));
    Geometry::draw();
}
