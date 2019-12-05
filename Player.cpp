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
};

glm::mat4 Player::getView() {
    return glm::lookAt(eye, eye + glm::vec3(sin(angle), (double)0, cos(angle)), glm::vec3(0, 1, 0));
}

void Player::draw() {
    if (turn_angle != 0) {
        std::cout<< angle << "[A]\n";
        angle += turn_angle * 0.02;
    }
    
    if (is_walking) {
        //smodel = glm::translate(eye);
        eye += glm::vec3(sin(angle), (double) 0, cos(angle)) / 2.0f;
    }
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(glm::translate(eye)));
    glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(0,1,0)));
    //Geometry::draw();
}
