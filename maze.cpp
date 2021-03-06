//
//  Maze.cpp
//  MAZERUNNER
//
//  Created by Gyuseung Hwang on 11/18/19.
//  Copyright © 2019 Gyuseung Hwang. All rights reserved.
//

#include "Maze.h"

std::vector<int> v;
std::vector<std::string> G;
int N;

int find(int a) {
    if (v[a] != a)    return v[a] = find(v[a]);
    return a;
}

void unions(int a, int b) {
    v[find(b)] = find(a);
}

bool canRemove(int i) {
    if (i % N == 0 || i % N == N - 1 || i / N == 0 || i / N == N - 1)
        return false;

    std::vector<int> v;
    if (G[i / N + 1][i % N] == ' ')
        v.push_back(i + N);
    if (G[i / N - 1][i % N] == ' ')
        v.push_back(i - N);
    if (G[i / N][i % N + 1] == ' ')
        v.push_back(i + 1);
    if (G[i / N][i % N - 1] == ' ')
        v.push_back(i - 1);

    if (v.size() <= 1){
        if (v.size() == 1) unions(v[0], i);
        return true;
    }
    sort(v.begin(), v.end());
    if (find(v.front()) == find(v.back()))   return false;

    for (int n : v){
        unions(n, i);
    }

    return true;
}

std::vector<std::string> generate() {
    G.resize(N, std::string(N, '#'));
    v.resize(N * N);

    for (int i = 0; i < N * N; i++) {
        v[i] = i;
    }

    std::vector<int> perm(N * N);

    for (int i = 0; i < N * N; i++) perm[i] = i;
    auto rng = std::default_random_engine {};
    shuffle(perm.begin(), perm.end(), rng);

    for (int i : perm) {
        if (canRemove(i)) {
            G[i / N][i % N] = ' ';
        }
    }
    return G;
}

Maze::Maze(GLuint shader, GLuint wallshader, std::string file_name) : Geometry(wallshader, file_name){
    srand((int)time(NULL));
    N = 50;
    generate();
  
    auto wall_object = new Geometry(wallshader, 0, 64);
    auto wall_frame = new Geometry(shader, "data/box.obj");
    floor = new Geometry(wallshader, 1, 128);
    shader = wallshader;
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            // remove walls for enterance and exit
            if (i + j > 1 && i + j < 2 * N - 1 && G[i][j] == '#') {
                walls.push_back(new Wall(i, j, rand(), wall_object, wall_frame));
            }
        }
    }
    model = glm::translate(glm::vec3(-500, -1, -500)) * glm::scale(glm::vec3(1000,1,1000));
}

void Maze::draw() {
//    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(glm::scale()));
    glUseProgram(shader);

    Geometry::draw(model, glm::vec3(0.5, 0.5, 0.5));

    for (auto w : walls) {
        w->draw(collision_on);
    }
}

bool Maze::collision(glm::vec3 position, float radius, bool unmark) {
    bool flag = false;
    for (auto w : walls) {
        auto wallPos = w->pos + glm::vec3(0.5, 0, 0.5);
        if (abs(wallPos.z - position.z) < (0.5 + radius) && abs(wallPos.x - position.x) < (0.5 + radius)) {
            flag = true;
            w->collided = true;
        }
        else
            if (w->collided && unmark) 
                w->collided = false;
    }
    return flag;
}
