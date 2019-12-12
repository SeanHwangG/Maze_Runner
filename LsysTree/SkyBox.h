#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <string>
#include "node.h"
#include "images.h"



class SkyBox : public Node
{
private:
	GLuint vao;
	GLuint vbos[2];

	std::vector<glm::ivec3> texCoord;

	std::vector<std::string> textureFiles
	{
		"skybox/right.jpg",
		"skybox/left.jpg",
		"skybox/top.jpg",
		"skybox/bottom.jpg",
		"skybox/front.jpg",
		"skybox/back.jpg"
	};



	GLuint textureID;
	GLuint programID;

	Camera *camera;

	GLuint modelLoc, viewLoc, projectionLoc;

public:
	SkyBox(float size, GLuint program, Camera* curCamera);
	~SkyBox();

	void draw(glm::mat4 C);
	void update() {};

	void spin(float deg);

	void loadSkyTexture(std::vector<std::string> textureFiles);
	GLuint getTextureID() { return textureID; }
};

#endif

