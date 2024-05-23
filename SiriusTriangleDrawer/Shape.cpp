#include "Shape.h"

bool inInterval(vec2 p, vec2 q, vec2 r) {
	float xMin, xMax;
	if (p.x <= r.x) { xMin = p.x; xMax = r.x; }
	else { xMin = r.x; xMax = p.x; }
	float yMin, yMax;
	if (p.y <= r.y) { yMin = p.y; yMax = r.y; }
	else { yMin = r.y; yMax = p.y; }
	if (q.x <= xMax && q.x >= xMin &&
		q.y <= yMax && q.y >= yMin)
		return true;
	return false;
}
int orientation(vec2 p, vec2 q, vec2 r) {
	float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
	if (val == 0) return 0;
	return (val > 0) ? 1 : 2;
}
bool doIntersect(vec2 p1, vec2 q1, vec2 p2, vec2 q2) {
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);
	if (o1 != o2 && o3 != o4) return true;
	if (o1 == 0 && inInterval(p1, p2, q1)) return true;
	if (o2 == 0 && inInterval(p1, q2, q1)) return true;
	if (o3 == 0 && inInterval(p2, p1, q2)) return true;
	if (o4 == 0 && inInterval(p2, q1, q2)) return true;
	return false;
}


void Shape::buffer() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
}
Shape::Shape(std::vector<float> vertexes) {
	color = randomRGB();
	buffer();
	setPoints(vertexes);
}
void Shape::addTriangle(vec2 p1, vec2 p2, vec2 p3) {
	triangles.push_back(p1.x);
	triangles.push_back(p1.y);
	triangles.push_back(p2.x);
	triangles.push_back(p2.y);
	triangles.push_back(p3.x);
	triangles.push_back(p3.y);
}
bool Shape::equalEndpoints(vec2 p1, vec2 p2, vec2 p3, vec2 p4) {
	bool b1 = false, b2 = false;
	if ((p1.x == p3.x && p1.y == p3.y) || (p1.x == p4.x && p1.y == p4.y) == true) b1 = true;
	if ((p2.x == p3.x && p2.y == p3.y) || (p2.x == p4.x && p2.y == p4.y) == true) b2 = true;
	if (b1 == true || b2 == true) return true;
	return false;
}
int Shape::intersectCount(vec2 p1, vec2 p2) {
	int counter = 0;
	for (int i = 0; i < points.size() - 1; i++) {
		if (!equalEndpoints(p1, p2, points.at(i), points.at(i + 1)))
			if (doIntersect(p1, p2, points.at(i), points.at(i + 1))) counter++;
	}
	if (!equalEndpoints(p1, p2, points.at(0), points.at(points.size() - 1)))
		if (doIntersect(p1, p2, points.at(0), points.at(points.size() - 1))) counter++;
	return counter;
}
void Shape::setPoints(std::vector<float> input) {
	std::vector<vec2> tmp;
	vec2 vector;
	for (int i = 0; i < input.size() - 2; i = i + 2) {
		vector = vec2(input.at(i), input.at(i + 1));
		tmp.push_back(vector);
		points.push_back(vector);
	}
	if (tmp.size() >= 3) {
		struct vec2 vecFirst, vecMiddle, vecLast;
		bool cutted = false;
		int first = -1, middle = 0, last = 1;
		while (tmp.size() > 3) {
			if (!cutted) {
				if (last == tmp.size() - 1) { first++; middle++; last = 0; }
				else if (middle == tmp.size() - 1) { first++;	middle = 0;	last++; }
				else if (first == tmp.size() - 1) { first = 0; middle++; last++; }
				else { first++;	middle++; last++; }
			}
			else {
				cutted = false;
				if (last == tmp.size()) last = 0;
				else if (middle == tmp.size()) { middle = 0; last++; }
				else if (first == tmp.size()) { first = 0; middle++; last++; }
			}
			vecFirst = tmp.at(first);
			vecMiddle = tmp.at(middle);
			vecLast = tmp.at(last);
			if (intersectCount(vecFirst, vecLast) == 0) {
				vec2 midPoint = (vecFirst + vecLast) / 2;
				vec2 infPoint = vec2(1.0, 1.0);
				if (intersectCount(midPoint, infPoint) % 2 == 1) {
					addTriangle(vecFirst, vecMiddle, vecLast);
					tmp.erase(tmp.begin() + middle);
					cutted = true;
				}
			}
		}
		addTriangle(tmp.at(0), tmp.at(1), tmp.at(2));
	}
}
void Shape::draw(GPUProgram* gpuProgram) {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 	// Copy to GPU target
		sizeof(float) * triangles.size(),  // # bytes
		&triangles.at(0),	      	// address
		GL_STATIC_DRAW);	// we do not change later
	glEnableVertexAttribArray(0);  // AttribArray 0
	glVertexAttribPointer(0,       // vbo -> AttribArray 0
		2, GL_FLOAT, GL_FALSE, // two floats/attrib, not fixed-point
		0, NULL); 		     // stride, offset: tightly packed
	glBindVertexArray(vao);  // Draw call
	gpuProgram->setUniform(color, "color");
	glDrawArrays(GL_TRIANGLES, 0 /*startIdx*/, triangles.size() / 2 /*# Elements*/);
}