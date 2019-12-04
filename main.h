#ifndef _MAIN_H_
#define _MAIN_H_
#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <GLUT/glut.h>

#include "OpenAL/al.h"
#include "OpenAL/alc.h"
#include "OpenAL/alut.h"
#include "Window.h"

#endif
