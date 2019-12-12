# include "images.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"

unsigned char* imread(char const* filename, int* x, int* y, int* comp, int req_comp)
{
	return stbi_load(filename, x, y, comp, req_comp);
}

void imfree(unsigned char* data)
{
	stbi_image_free(data);
}