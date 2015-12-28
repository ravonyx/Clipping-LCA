#include <GL\glew.h>

#ifdef _WIN32
#pragma comment(lib, "opengl32.lib")
#include <Windows.h>
#endif

#include <GL\freeglut.h>
#include <glm\glm.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iterator>


GLint referenceX = 0; //Declar reference points used to drag 
GLint referenceY = 0; // the rectangle 
GLfloat xPosition = 300.0;
GLfloat yPosition = 220.0;
GLsizei winWidth = 400, winHeight = 400, shapeSize = 50;
GLfloat red = 1.0f, blue = 0.0f, green = 0.0f;
#define FALSE 0
#define TRUE 1

int up = FALSE;
int down = FALSE;
int left = FALSE;
int right = FALSE;

bool finish_window = FALSE;
bool finish_poly = FALSE;
bool finish_fenetrage = FALSE;

struct Point
{
	Point(float x, float y) : x(x), y(y) {}
	float x, y;
};
std::vector< Point > points_window;
std::vector< Point > points_poly;
std::vector< Point > points_solution;

void Initialize()
{
	printf("Version OpenGL : %s\n", glGetString(GL_VERSION));
	printf("Fabricant : %s\n", glGetString(GL_VENDOR));
	printf("Pilote : %s\n", glGetString(GL_RENDERER));
	printf("Version GLSL : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glewInit();
	//glGetStringi = (PFNGLGETSTRINGIPROC)wglGetProcAddress("glGetStringi");
	int numExtensions;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
	for (auto index = 0; index < numExtensions; ++index)
		printf("Extensions[%d] : %s\n", index, glGetStringi(GL_EXTENSIONS, index));
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		if (!finish_window)
			points_window.push_back(Point(x, y));
		if (!finish_poly && finish_window)
			points_poly.push_back(Point(x, y));
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		points_window.clear();
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) 
	{
		case 27: exit(0); break;
		case 32:
			if (finish_window)
				finish_poly = TRUE;
			finish_window = TRUE;
			break;
	}
	glutPostRedisplay();
}

void drawLines(std::vector< Point > points)
{
	for (size_t i = 0; i < points.size(); ++i)
	{
		if (i > 0)
		{
			const Point& ptStart = points[i - 1];
			const Point& ptEnd = points[i];
			glBegin(GL_LINES);
			glVertex3f(ptStart.x, ptStart.y, 0);
			glVertex3f(ptEnd.x, ptEnd.y, 0);
			glEnd();
		}

		if (i == points.size() - 1)
		{
			const Point& ptStart = points[i];
			const Point& ptEnd = points[0];
			glBegin(GL_LINES);
			glVertex3f(ptStart.x, ptStart.y, 0);
			glVertex3f(ptEnd.x, ptEnd.y, 0);
			glEnd();
		}
	}
}

void drawPoints(std::vector< Point > points)
{
	glBegin(GL_QUADS);
	for (size_t i = 0; i < points.size(); ++i)
	{
		const unsigned int SIZE = 5;
		const Point& pt = points[i];
		glVertex2i(pt.x - SIZE, pt.y - SIZE);
		glVertex2i(pt.x + SIZE, pt.y - SIZE);
		glVertex2i(pt.x + SIZE, pt.y + SIZE);
		glVertex2i(pt.x - SIZE, pt.y + SIZE);
	}
	glEnd();
}


bool isVisble(Point point)
{
	float minX = points_window[0].x;
	float maxX = points_window[0].x;
	float minY = points_window[0].y;
	float maxY = points_window[0].y;
	for (size_t i = 1; i < points_window.size(); i++)
	{
		Point q = points_window[i];
		minX = std::min(q.x, minX);
		maxX = std::max(q.x, maxX);
		minY = std::min(q.y, minY);
		maxY = std::max(q.y, maxY);
	}

	//Check the extreme border
	if (point.x < minX || point.x > maxX || point.y < minY || point.y > maxY)
	{
		return false;
	}

	bool inside = false;
	for (size_t i = 0, j = points_window.size() - 1; i < points_window.size(); j = i++)
	{
		if ((points_window[i].y > point.y) != (points_window[j].y > point.y) && point.x < (points_window[j].x - points_window[i].x) * (point.y - points_window[i].y) / (points_window[j].y - points_window[i].y) + points_window[i].x)
		{
			inside = !inside;
		}
	}

	return inside;
}

glm::vec2 cut(glm::vec2 line1, glm::vec2 line2)
{
	glm::vec2 v3;
	
	return v3;
}

void Fenetrage()
{
	for (size_t i = 0; i < points_window.size(); i++)
	{
		for (size_t i = 0; i < points_poly.size(); i++)
		{
			if (isVisble(points_poly[i]))
				points_solution.push_back(points_poly[i]);
			else
			{
				Point a = points_poly[i - 1];
				Point b = points_poly[i];
				glm::vec2 linePoly = { b.x - a.x, b.y - a.y };

				Point a_window = points_window[i - 1];
				Point b_window = points_window[i];
				glm::vec2 lineWindow = { b_window.x - a_window.x, b_window.y - a_window.y };

				glm::vec2 point = cut(linePoly, lineWindow);
				printf("%d %d", point.r, point.s);
			}
		}
	}

	finish_fenetrage = TRUE;
}

void display(void)
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	glOrtho(0, w, h, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3ub(255, 0, 0);
	drawPoints(points_window);
	if (finish_window)
		drawLines(points_window);

	glColor3ub(0, 255, 0);
	drawPoints(points_poly);
	if (finish_poly)
		drawLines(points_poly);

	if (finish_poly)
		Fenetrage();

	if (finish_fenetrage)
	{
		points_poly.clear();
		drawPoints(points_solution);
		drawLines(points_solution);
	}
	
	glutSwapBuffers();

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL");

	Initialize();
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}

