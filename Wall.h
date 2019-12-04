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

class Wall : public Geometry
{
public:
	Wall(GLuint shader, int i, int j, int height);
	~Wall();

	void draw();
};

#endif

