#ifndef _SHAPE_
#define _SHAPE_

#include "framework.h"

bool inInterval(vec2 p, vec2 q, vec2 r);
int orientation(vec2 p, vec2 q, vec2 r);
bool doIntersect(vec2 p1, vec2 q1, vec2 p2, vec2 q2);

class Shape {
private:
	std::vector<float> triangles;
	std::vector<vec2> points;
	unsigned int vao;
	unsigned int vbo;
	vec3 color;
public:
	void buffer();
	Shape(std::vector<float> vertexes);
	void addTriangle(vec2 p1, vec2 p2, vec2 p3);
	bool equalEndpoints(vec2 p1, vec2 p2, vec2 p3, vec2 p4);
	int intersectCount(vec2 p1, vec2 p2);
	void setPoints(std::vector<float> input);
	void draw(GPUProgram* gpuProgram);
};


#endif