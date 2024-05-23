#ifndef _TRIANGLE_
#define _TRIANGLE_

#include "framework.h"

class Triangle {
private:
	unsigned int vao;
	unsigned int vbo;
	std::vector<float> points;
	vec3 color;
public:
	void buffer();
	Triangle(float aX, float aY, float bX, float bY, float cX, float cY);
	std::vector<float> getPoints() const;
	void draw(GPUProgram* gpuProgram);
	float quickMathY(float xa, float ya, float xb, float yb, float xc, float yc);
	float quickMathX(float xa, float ya, float xc, float yc, float y);
	float getAngle(float x1, float y1, float x2, float y2);
	bool isOrigoCrossed(float v1x, float v1y, float v2x, float v2y);
	float getFirstAngle(float x1, float y1, float x2, float y2, float angle);
	void addLine(float p1x, float p1y, float p2x, float p2y);
	void addCurve(float p1x, float p1y, float p2x, float p2y);
	void addPoints(float aX, float aY, float bX, float bY);
	void clearDuplicates();
};

#endif