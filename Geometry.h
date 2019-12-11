#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_
#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "stb_image.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <time.h>


class Geometry
{
public:
    double INF = 99999;
    GLuint textureID;
    std::vector<glm::vec3> points, normals, out_vertices, out_normals;
    std::vector<glm::ivec3> indices;
    glm::mat4 model;
    GLuint vaos[2], vbos[2], ebo, shader;
    Geometry();
    Geometry(GLuint shader);
    Geometry(GLuint shader, std::string objFilename);
    ~Geometry();
    
    void draw();
    void draw(glm::mat4 M, glm::vec3 color);
    void update(){};
    static GLuint loadCubemap(std::vector<std::string> faces);
};

#endif

