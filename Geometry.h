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
#include <stb_image.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <time.h>

#include "Object.h"

class Geometry : public Object
{
public:
    double INF = 99999;
    GLuint textureID;
    std::vector<glm::vec3> points, normals, out_vertices, out_normals;
    std::vector<glm::ivec3> indices;
    GLuint vao, vbos[2], ebo, shader;
    Geometry();
    Geometry(GLuint shader);
    Geometry(std::string objFilename, GLuint shader);
    ~Geometry();
    
    void draw();
    void update(){};
    static GLuint loadCubemap(std::vector<std::string> faces);
};

#endif

