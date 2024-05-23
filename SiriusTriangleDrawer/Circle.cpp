#include "Circle.h"

//const int steps = 120;

void Circle::buffer() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
}
Circle::Circle(float x, float y, float radius, vec3 rgb) {
	buffer();
	color = rgb;
	float rad = radius;
	centX = x;
	centY = y;
	float prevX = centX;
	float prevY = centY - rad;
	float nextX = centX + rad * sin(2.0f * M_PI / steps);
	float nextY = centY - rad * cos(2.0f * M_PI / steps);
	for (int i = 0; i < steps + 1; i++) {
		float phi = (i + 1) * (2.0f * M_PI / steps);
		vertices[i * 6 + 0] = centX;
		vertices[i * 6 + 1] = centY;
		vertices[i * 6 + 2] = prevX;
		vertices[i * 6 + 3] = prevY;
		vertices[i * 6 + 4] = nextX;
		vertices[i * 6 + 5] = nextY;
		prevX = nextX;
		prevY = nextY;
		nextX = centX + rad * sin(phi);
		nextY = centY - rad * cos(phi);
	}
}
void Circle::draw(GPUProgram* gpuProgram) {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 	// Copy to GPU target
		sizeof(vertices),  // # bytes
		vertices,	      	// address
		GL_STATIC_DRAW);	// we do not change later
	glEnableVertexAttribArray(0);  // AttribArray 0
	glVertexAttribPointer(0,       // vbo -> AttribArray 0
		2, GL_FLOAT, GL_FALSE, // two floats/attrib, not fixed-point
		0, NULL); 		     // stride, offset: tightly packed
	glBindVertexArray(vao);  // Draw call
	gpuProgram->setUniform(color, "color");
	glDrawArrays(GL_TRIANGLES, 0 /*startIdx*/, steps * 3 + 3 /*# Elements*/);
}