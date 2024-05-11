//=============================================================================================
// Mintaprogram: Zöld háromszög. Ervenyes 2018. osztol.
//
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat, BOM kihuzando.
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni a printf-et kiveve
// - Mashonnan atvett programresszleteket forrasmegjeloles nelkul felhasznalni es
// - felesleges programsorokat a beadott programban hagyni!!!!!!! 
// - felesleges kommenteket a beadott programba irni a forrasmegjelolest kommentjeit kiveve
// ---------------------------------------------------------------------------------------------
// A feladatot ANSI C++ nyelvu forditoprogrammal ellenorizzuk, a Visual Studio-hoz kepesti elteresekrol
// es a leggyakoribb hibakrol (pl. ideiglenes objektumot nem lehet referencia tipusnak ertekul adni)
// a hazibeado portal ad egy osszefoglalot.
// ---------------------------------------------------------------------------------------------
// A feladatmegoldasokban csak olyan OpenGL fuggvenyek hasznalhatok, amelyek az oran a feladatkiadasig elhangzottak 
// A keretben nem szereplo GLUT fuggvenyek tiltottak.
//
// NYILATKOZAT
// ---------------------------------------------------------------------------------------------
// Nev    : Horváth Viktor
// Neptun : GSKRCV
// ---------------------------------------------------------------------------------------------
// ezennel kijelentem, hogy a feladatot magam keszitettem, es ha barmilyen segitseget igenybe vettem vagy
// mas szellemi termeket felhasznaltam, akkor a forrast es az atvett reszt kommentekben egyertelmuen jeloltem.
// A forrasmegjeloles kotelme vonatkozik az eloadas foliakat es a targy oktatoi, illetve a
// grafhazi doktor tanacsait kiveve barmilyen csatornan (szoban, irasban, Interneten, stb.) erkezo minden egyeb
// informaciora (keplet, program, algoritmus, stb.). Kijelentem, hogy a forrasmegjelolessel atvett reszeket is ertem,
// azok helyessegere matematikai bizonyitast tudok adni. Tisztaban vagyok azzal, hogy az atvett reszek nem szamitanak
// a sajat kontribucioba, igy a feladat elfogadasarol a tobbi resz mennyisege es minosege alapjan szuletik dontes.
// Tudomasul veszem, hogy a forrasmegjeloles kotelmenek megsertese eseten a hazifeladatra adhato pontokat
// negativ elojellel szamoljak el es ezzel parhuzamosan eljaras is indul velem szemben.
//=============================================================================================
#include "framework.h"

// vertex shader in GLSL: It is a Raw string (C++11) since it contains new line characters
const char* const vertexSource = R"(
	#version 330				// Shader 3.3
	precision highp float;		// normal floats, makes no difference on desktop computers

	uniform mat4 MVP;			// uniform variable, the Model-View-Projection transformation matrix
	layout(location = 0) in vec2 vp;	// Varying input: vp = vertex position is expected in attrib array 0

	void main() {
		gl_Position = vec4(vp.x, vp.y, 0, 1) * MVP;		// transform vp from modeling space to normalized device space
	}
)";

// fragment shader in GLSL
const char* const fragmentSource = R"(
	#version 330			// Shader 3.3
	precision highp float;	// normal floats, makes no difference on desktop computers
	
	uniform vec3 color;		// uniform variable, the color of the primitive
	out vec4 outColor;		// computed color of the current pixel

	void main() {
		outColor = vec4(color, 1);	// computed color is the color of the primitive
	}
)";

GPUProgram gpuProgram; // vertex and fragment shaders
unsigned int vao;	   // virtual world on the GPU
const int steps = 120;
int mouseClick = 0;

vec3 randomColor() {
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	return vec3(r, g, b);
}

class Circle {
private:
	unsigned int vao;
	unsigned int vbo;
	float vertices[steps * 6 + 6];
	float centX;
	float centY;
	vec3 color;
public:
	void buffer() {
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
	}
	Circle(float x, float y, float radius, vec3 rgb) {
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
	void draw() {
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
		gpuProgram.setUniform(color, "color");
		glDrawArrays(GL_TRIANGLES, 0 /*startIdx*/, steps * 3 + 3 /*# Elements*/);
	}
};

class Triangle {
private:
	unsigned int vao;
	unsigned int vbo;
	std::vector<float> points;
	vec3 color;
public:
	void buffer() {
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
	}
	Triangle(float aX, float aY, float bX, float bY, float cX, float cY) {
		color = randomColor();
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
	std::vector<float> getPoints() {
		return points;
	}
	void draw() {
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
		gpuProgram.setUniform(color, "color");
		glDrawArrays(GL_LINE_STRIP, 0 /*startIdx*/, points.size() / 2 /*# Elements*/);
	}
	float quickMathY(float xa, float ya, float xb, float yb, float xc, float yc) {
		if (xa == xb) return ((ya + yb) / 2);
		if (xa == xc) return ((ya + yc) / 2);
		if (xc == xb) return ((yc + yb) / 2);
		float p1 = (xb * xb - xa * xa + yb * yb - ya * ya) / (2 * (xb - xa));
		float p2 = (xc * xc - xa * xa + yc * yc - ya * ya) / (2 * (xc - xa));
		float p3 = ((yb - ya) / (xb - xa)) - ((yc - ya) / (xc - xa));
		return ((p1 - p2) / p3);
	}
	float quickMathX(float xa, float ya, float xc, float yc, float y) {
		float p1 = (-1) * xc * xc + xa * xa - yc * yc + ya * ya + 2 * (yc - ya) * y;
		float p2 = (-2) * (xc - xa);
		return (p1 / p2);
	}
	float getAngle(float x1, float y1, float x2, float y2) {
		return acos((x1 * x2 + y1 * y2) / (sqrt(x1 * x1 + y1 * y1) * sqrt(x2 * x2 + y2 * y2))) * (180 / M_PI);
	}
	bool isOrigoCrossed(float v1x, float v1y, float v2x, float v2y) {
		if (fabs(v1x) < 0.01 && fabs(v2x) < 0.01 || fabs(v1y) < 0.01 && fabs(v2y) < 0.01 || fabs(v1x) < 0.01 && fabs(v1y) < 0.01 || fabs(v2x) < 0.01 && fabs(v2y) < 0.01) {
			return true;
		}
		return fabs((v1x / v2x) * v2y - v1y) < 0.01;
	}
	float getFirstAngle(float x1, float y1, float x2, float y2, float angle) {
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
	void addLine(float p1x, float p1y, float p2x, float p2y) {
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
	void addCurve(float p1x, float p1y, float p2x, float p2y) {
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
	void addPoints(float aX, float aY, float bX, float bY) {
		if (!isOrigoCrossed(aX, aY, bX, bY)) {
			addCurve(aX, aY, bX, bY);
		}
		else {
			addLine(aX, aY, bX, bY);
		}
	}
	void clearDuplicates() {
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
};

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

class Shape {
private:
	std::vector<float> triangles;
	std::vector<vec2> points;
	unsigned int vao;
	unsigned int vbo;
	vec3 color;
public:
	void buffer() {
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
	}
	Shape(std::vector<float> vertexes) {
		color = randomColor();
		buffer();
		setPoints(vertexes);
	}
	void addTriangle(vec2 p1, vec2 p2, vec2 p3) {
		triangles.push_back(p1.x);
		triangles.push_back(p1.y);
		triangles.push_back(p2.x);
		triangles.push_back(p2.y);
		triangles.push_back(p3.x);
		triangles.push_back(p3.y);
	}
	bool equalEndpoints(vec2 p1, vec2 p2, vec2 p3, vec2 p4) {
		bool b1 = false, b2 = false;
		if ((p1.x == p3.x && p1.y == p3.y) || (p1.x == p4.x && p1.y == p4.y) == true) b1 = true;
		if ((p2.x == p3.x && p2.y == p3.y) || (p2.x == p4.x && p2.y == p4.y) == true) b2 = true;
		if (b1 == true || b2 == true) return true;
		return false;
	}
	int intersectCount(vec2 p1, vec2 p2) {
		int counter = 0;
		for (int i = 0; i < points.size() - 1; i++) {
			if (!equalEndpoints(p1, p2, points.at(i), points.at(i + 1)))
				if (doIntersect(p1, p2, points.at(i), points.at(i + 1))) counter++;
		}
		if (!equalEndpoints(p1, p2, points.at(0), points.at(points.size() - 1)))
			if (doIntersect(p1, p2, points.at(0), points.at(points.size() - 1))) counter++;
		return counter;
	}
	void setPoints(std::vector<float> input) {
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
	void draw() {
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
		gpuProgram.setUniform(color, "color");
		glDrawArrays(GL_TRIANGLES, 0 /*startIdx*/, triangles.size() / 2 /*# Elements*/);
	}
};

std::vector<Circle*> baseCircle;
std::vector<Circle*> mousePoints;
std::vector<Triangle*> triangles;
std::vector<Shape*> shapes;
float floats[6];

// Initialization, create an OpenGL context
void onInitialization() {
	glViewport(0, 0, windowWidth, windowHeight);

	baseCircle = { new Circle(0.0f, 0.0f, 1.0f, vec3(0.4f, 0.4f, 0.4f)) };

	// create program for the GPU
	gpuProgram.create(vertexSource, fragmentSource, "outColor");
}

// Window has become invalid: Redraw
void onDisplay() {
	glClearColor(0, 0, 0, 0);     // background color
	glClear(GL_COLOR_BUFFER_BIT); // clear frame buffer

	// Set color to (0, 1, 0) = green
	int location = glGetUniformLocation(gpuProgram.getId(), "color");

	float MVPtransf[4][4] = { 1, 0, 0, 0,    // MVP matrix, 
							  0, 1, 0, 0,    // row-major!
							  0, 0, 1, 0,
							  0, 0, 0, 1 };

	location = glGetUniformLocation(gpuProgram.getId(), "MVP");	// Get the GPU location of uniform variable MVP
	glUniformMatrix4fv(location, 1, GL_TRUE, &MVPtransf[0][0]);	// Load a 4x4 row-major float matrix to the specified location

	baseCircle.at(0)->draw();
	int mP = 0;
	for (int i = 0; i < triangles.size(); i++) {
		for (int j = 0; j < 3; j++)
			mousePoints.at(mP++)->draw();
		shapes.at(i)->draw();
		triangles.at(i)->draw();
	}
	for (int i = mP; i < mousePoints.size(); i++)
		mousePoints.at(mP++)->draw();

	glutSwapBuffers(); // exchange buffers for double buffering
}

// Key of ASCII code pressed
void onKeyboard(unsigned char key, int pX, int pY) {
	if (key == 'd') glutPostRedisplay();         // if d, invalidate display, i.e. redraw
}

// Key of ASCII code released
void onKeyboardUp(unsigned char key, int pX, int pY) {
}

// Move mouse with key pressed
void onMouseMotion(int pX, int pY) {	// pX, pY are the pixel coordinates of the cursor in the coordinate system of the operation system
	// Convert to normalized device space
	float cX = 2.0f * pX / windowWidth - 1;	// flip y axis
	float cY = 1.0f - 2.0f * pY / windowHeight;
	//printf("Mouse moved to (%3.2f, %3.2f)\n", cX, cY);
}

void onMouse(int button, int state, int pX, int pY) { // pX, pY are the pixel coordinates of the cursor in the coordinate system of the operation system
	// Convert to normalized device space
	float cX = 2.0f * pX / windowWidth - 1;	// flip y axis
	float cY = 1.0f - 2.0f * pY / windowHeight;

	const char* buttonStat;
	switch (state) {
	case GLUT_DOWN: buttonStat = "pressed"; break;
	case GLUT_UP:   buttonStat = "released"; break;
	}

	switch (button) {
	case GLUT_LEFT_BUTTON:
		//printf("Left button %s at (%3.2f, %3.2f)\n", buttonStat, cX, cY);
		if (mouseClick < 3) {
			if (sqrt(cX * cX + cY * cY < 1.0f)) {
				if (state == GLUT_DOWN) {
					if (mouseClick == 0) {
						floats[0] = cX;	floats[1] = cY;
					}
					if (mouseClick == 1) {
						if (floats[0] != cX && floats[1] != cY) {
							floats[2] = cX;	floats[3] = cY;
						}
					}
					if (mouseClick == 2) {
						if (floats[2] != cX && floats[3] != cY) {
							floats[4] = cX; floats[5] = cY;
						}
					}
					mousePoints.push_back(new Circle(cX, cY, 0.01f, vec3(1.0f, 0.0f, 0.0f)));
					mouseClick++;
					if (mouseClick == 3) {
						triangles.push_back(new Triangle(floats[0], floats[1], floats[2], floats[3], floats[4], floats[5]));
						shapes.push_back(new Shape(triangles.at(triangles.size() - 1)->getPoints()));
						mouseClick = 0;
					}
					glutPostRedisplay();
				}
			}
		}
		break;
	case GLUT_MIDDLE_BUTTON: /*printf("Middle button %s at (%3.2f, %3.2f)\n", buttonStat, cX, cY);*/ break;
	case GLUT_RIGHT_BUTTON:  /*printf("Right button %s at (%3.2f, %3.2f)\n", buttonStat, cX, cY);*/  break;
	}
}

// Idle event indicating that some time elapsed: do animation here
void onIdle() {
	long time = glutGet(GLUT_ELAPSED_TIME); // elapsed time since the start of the program
}
