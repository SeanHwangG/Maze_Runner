#ifndef _IMAGES_H_



unsigned char* imread(char const* filename, int* x, int* y, int* comp, int req_comp);

void imfree(unsigned char* data);


#endif
