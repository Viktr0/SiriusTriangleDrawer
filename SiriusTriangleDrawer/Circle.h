#ifndef _CIRCLE_ 
#define _CIRCLE_ 

#include "framework.h"

class Circle {
private:
	unsigned int vao;
	unsigned int vbo;
	float vertices[steps * 6 + 6];
	float centX;
	float centY;
	vec3 color;
public:
	void buffer();
	Circle(float x, float y, float radius, vec3 rgb);
	void draw(GPUProgram* gpuProgram);
};

#endif