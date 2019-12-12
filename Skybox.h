#ifndef _SKYBOX_H_
#define _SKYBOX_H_
#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Geometry.h"

class Skybox : public Geometry
{
public:
    Skybox(GLuint shader);
    ~Skybox();
    void draw();
	void draw(glm::mat4 C) {}
	void update() {}
};

#endif

