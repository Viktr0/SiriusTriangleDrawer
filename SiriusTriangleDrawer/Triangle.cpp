#include "Triangle.h"


void Triangle::buffer() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
}
Triangle::Triangle(float aX, float aY, float bX, float bY, float cX, float cY) {
	color = randomRGB();
	buffer();
	addPoints(aX, aY, bX, bY);
	if (fabs((1000 * points.at(points.size() - 2) - 1000 * aX)) < 5 && fabs((1000 * points.at(points.size() - 1) - 1000 * aY)) < 5) {
		addPoints(aX, aY, cX, cY);
		addPoints(bX, bY, cX, cY);
	}
	else {
		addPoints(bX, bY, cX, cY);
		addPoints(aX, aY, cX, cY);
	}
	clearDuplicates();
}
std::vector<float> Triangle::getPoints() const {
	return points;
}
void Triangle::draw(GPUProgram* gpuProgram) {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 	// Copy to GPU target
		sizeof(float) * points.size(),  // # bytes
		&points.at(0),	      	// address
		GL_STATIC_DRAW);	// we do not change later
	glLineWidth(1.4f);
	glEnableVertexAttribArray(0);  // AttribArray 0
	glVertexAttribPointer(0,       // vbo -> AttribArray 0
		2, GL_FLOAT, GL_FALSE, // two floats/attrib, not fixed-point
		0, NULL); 		     // stride, offset: tightly packed
	glBindVertexArray(vao);  // Draw call
	gpuProgram->setUniform(color, "color");
	glDrawArrays(GL_LINE_STRIP, 0 /*startIdx*/, points.size() / 2 /*# Elements*/);
}
float Triangle::quickMathY(float xa, float ya, float xb, float yb, float xc, float yc) {
	if (xa == xb) return ((ya + yb) / 2);
	if (xa == xc) return ((ya + yc) / 2);
	if (xc == xb) return ((yc + yb) / 2);
	float p1 = (xb * xb - xa * xa + yb * yb - ya * ya) / (2 * (xb - xa));
	float p2 = (xc * xc - xa * xa + yc * yc - ya * ya) / (2 * (xc - xa));
	float p3 = ((yb - ya) / (xb - xa)) - ((yc - ya) / (xc - xa));
	return ((p1 - p2) / p3);
}
float Triangle::quickMathX(float xa, float ya, float xc, float yc, float y) {
	float p1 = (-1) * xc * xc + xa * xa - yc * yc + ya * ya + 2 * (yc - ya) * y;
	float p2 = (-2) * (xc - xa);
	return (p1 / p2);
}
float Triangle::getAngle(float x1, float y1, float x2, float y2) {
	return acos((x1 * x2 + y1 * y2) / (sqrt(x1 * x1 + y1 * y1) * sqrt(x2 * x2 + y2 * y2))) * (180 / M_PI);
}
bool Triangle::isOrigoCrossed(float v1x, float v1y, float v2x, float v2y) {
	if (fabs(v1x) < 0.01 && fabs(v2x) < 0.01 || fabs(v1y) < 0.01 && fabs(v2y) < 0.01 || fabs(v1x) < 0.01 && fabs(v1y) < 0.01 || fabs(v2x) < 0.01 && fabs(v2y) < 0.01) {
		return true;
	}
	return fabs((v1x / v2x) * v2y - v1y) < 0.01;
}
float Triangle::getFirstAngle(float x1, float y1, float x2, float y2, float angle) {
	angle = angle * M_PI / 180;
	float x1r = cos(angle) * x1 - sin(angle) * y1;
	float y1r = sin(angle) * x1 + cos(angle) * y1;
	float x2r = cos(angle) * x2 - sin(angle) * y2;
	float y2r = sin(angle) * x2 + cos(angle) * y2;
	float elsoElforgatott = sqrt((x1r - x2) * (x1r - x2) + (y1r - y2) * (y1r - y2));
	float masodikElforgatott = sqrt((x2r - x1) * (x2r - x1) + (y2r - y1) * (y2r - y1));
	if (elsoElforgatott < masodikElforgatott) {
		if (x1 < 0) return (360 - getAngle(x1, y1, 0, -1));
		else return getAngle(x1, y1, 0, -1);
	}
	else {
		if (x2 < 0) return (360 - getAngle(x2, y2, 0, -1));
		else return getAngle(x2, y2, 0, -1);
	}
}
void Triangle::addLine(float p1x, float p1y, float p2x, float p2y) {
	if (points.size() != 0) {
		float d1 = fabs((points.at(points.size() - 2) - p1x)) + fabs((points.at(points.size() - 1) - p1y));
		float d2 = fabs((points.at(points.size() - 2) - p2x)) + fabs((points.at(points.size() - 1) - p2y));
		if (d1 > d2) {
			float tmpX = p1x;
			float tmpY = p1y;
			p1x = p2x;
			p1y = p2y;
			p2x = tmpX;
			p2y = tmpY;
		}
	}
	points.push_back(p1x);
	points.push_back(p1y);
	points.push_back(p2x);
	points.push_back(p2y);
}
void Triangle::addCurve(float p1x, float p1y, float p2x, float p2y) {
	float invAX = p1x * (1 / (p1x * p1x + p1y * p1y));
	float invAY = p1y * (1 / (p1x * p1x + p1y * p1y));
	float centerY = quickMathY(p1x, p1y, p2x, p2y, invAX, invAY);
	float centerX;
	if (p1x - invAX == 0) centerX = quickMathX(p2x, p2y, invAX, invAY, centerY);
	else centerX = quickMathX(p1x, p1y, invAX, invAY, centerY);
	float radX = p1x - centerX;
	float radY = p1y - centerY;
	float rad = sqrt(radX * radX + radY * radY);
	float angle = getAngle(p1x - centerX, p1y - centerY, p2x - centerX, p2y - centerY);
	float prevX, prevY, nextX, nextY;
	float scale = 360 / steps;
	float startAngle = getFirstAngle(p1x - centerX, p1y - centerY, p2x - centerX, p2y - centerY, angle);
	float endAngle = startAngle + angle;
	const int intStart = (int)(startAngle / scale + 1);
	const int intEnd = (int)(endAngle / scale - 1);
	std::vector<float> tmp;
	float phi = startAngle / scale * (2.0f * M_PI / steps);
	prevX = centerX + rad * sin(phi);
	prevY = centerY - rad * cos(phi);
	float d1 = fabs((prevX - p1x)) + fabs((prevY - p1y));
	float d2 = fabs((prevX - p2x)) + fabs((prevY - p2y));
	if (d1 < d2) {
		prevX = p1x;
		prevY = p1y;
	}
	else {
		prevX = p2x;
		prevY = p2y;
	}
	tmp.push_back(prevX);
	tmp.push_back(prevY);
	for (int i = intStart; i < intEnd; i++) {
		float phi = (i + 1) * (2.0f * M_PI / steps);
		nextX = centerX + rad * sin(phi);
		nextY = centerY - rad * cos(phi);
		tmp.push_back(prevX);
		tmp.push_back(prevY);
		prevX = nextX;
		prevY = nextY;
	}
	float alpha = endAngle / scale * (2.0f * M_PI / steps);
	d1 = fabs((prevX - p1x)) + fabs((prevY - p1y));
	d2 = fabs((prevX - p2x)) + fabs((prevY - p2y));
	if (d1 < d2) {
		nextX = p1x;
		nextY = p1y;
	}
	else {
		nextX = p2x;
		nextY = p2y;
	}
	nextX = centerX + rad * sin(alpha);
	nextY = centerY - rad * cos(alpha);
	tmp.push_back(nextX);
	tmp.push_back(nextY);
	if (points.size() != 0) {
		if (fabs(100 * tmp.at(0) - 100 * points.at(points.size() - 2)) < 1 && fabs(100 * tmp.at(1) - 100 * points.at(points.size() - 1)) < 1) {
			for (int i = 0; i < tmp.size(); i++)
				points.push_back(tmp.at(i));
		}
		else {
			for (int i = tmp.size() - 2; i >= 0; i = i - 2) {
				points.push_back(tmp.at(i));
				points.push_back(tmp.at(i + 1));
			}
		}
	}
	else {
		for (int i = 0; i < tmp.size(); i++)
			points.push_back(tmp.at(i));
	}
}
void Triangle::addPoints(float aX, float aY, float bX, float bY) {
	if (!isOrigoCrossed(aX, aY, bX, bY)) {
		addCurve(aX, aY, bX, bY);
	}
	else {
		addLine(aX, aY, bX, bY);
	}
}
void Triangle::clearDuplicates() {
	if (points.size() != 0) {
		int y = 1;
		for (int x = 0; x < points.size() - 3; x++) {
			if (points.size() > y + 2) {
				if (fabs(1000 * points.at(x) - 1000 * points.at(x + 2)) < 1 && fabs(1000 * points.at(y) - 1000 * points.at(y + 2)) < 1) {
					points.erase(points.begin() + x);
					points.erase(points.begin() + x);
					x--; y--;
				}
				y++;
			}
		}
	}
}