#ifndef _CUBE_H_
#define _CUBE_H_
#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Geometry.h"

class Wall
{
public:
	Wall(int i, int j, int height, Geometry* wall_object, Geometry* wall_frame);
	glm::vec3 pos;
	glm::mat4 model;
	bool collided = false;
	Geometry* wall_object;
	Geometry* wall_frame;
	~Wall();

	void draw(bool collision_on);
};

#endif

