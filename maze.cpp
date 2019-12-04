//
//  Maze.cpp
//  MAZERUNNER
//
//  Created by Gyuseung Hwang on 11/18/19.
//  Copyright © 2019 Gyuseung Hwang. All rights reserved.
//

#include "Maze.h"
#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <time.h>
#include <random>
#include <algorithm>
#include <set>

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

Maze::Maze(GLuint shader, std::string file_name) : Geometry(shader){
    srand((int)time(NULL));
    N = 50;
    generate();
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (G[i][j] == '#') {
                walls.push_back(new Wall(shader, i, j, rand()));
            }
        }
    }
}

void Maze::draw() {
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(0,0,0)));
    for (auto w : walls) {
        w->draw();
    }
}
