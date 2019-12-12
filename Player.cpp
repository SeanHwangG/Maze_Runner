//
//  Player.cpp
//  MAZERUNNER
//
//  Created by Gyuseung Hwang on 11/18/19.
//  Copyright © 2019 Gyuseung Hwang. All rights reserved.
//

#include "Player.h"

Player::Player(GLuint shader, std::string objFilename, glm::vec3 start_pos, Maze* maz) : Geometry(shader, objFilename){
    eye = start_pos;
    this->maz = maz;
};

glm::mat4 Player::getView() {
    return glm::lookAt(eye, eye + glm::vec3(sin(angle), (double)0, cos(angle)), glm::vec3(0, 1, 0));
}

void Player::draw() {
    if (turn_angle != 0) {
        //std::cout<< angle << "[A]\n";
        angle += turn_angle * 0.035;
    }
    
    if (is_walking) {
        //smodel = glm::translate(eye);
        if (!maz->collision(eye + glm::vec3(sin(angle), 0, 0) / 20.0f, radius, true))
            eye += glm::vec3(sin(angle), 0, 0) / 20.0f;
        if (!maz->collision(eye + glm::vec3(0, 0, cos(angle)) / 20.0f, radius, false))
            eye += glm::vec3(0, 0, cos(angle)) / 20.0f;
    }
    if (is_backing) {
        //eye -= glm::vec3(sin(angle), (double)0, cos(angle)) / 20.0f;
        if (!maz->collision(eye - glm::vec3(sin(angle), 0, 0) / 20.0f, radius, true))
            eye -= glm::vec3(sin(angle), 0, 0) / 20.0f;
        if (!maz->collision(eye - glm::vec3(0, 0, cos(angle)) / 20.0f, radius, false))
            eye -= glm::vec3(0, 0, cos(angle)) / 20.0f;
    }
    model = glm::translate(eye) * glm::scale(glm::vec3(radius, radius, radius));
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(1,1,1)));
    Geometry::draw();
}
