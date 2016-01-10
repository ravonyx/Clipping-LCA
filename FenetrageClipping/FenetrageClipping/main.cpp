#include <GL\glew.h>

#ifdef _WIN32
#pragma comment(lib, "opengl32.lib")
#include <Windows.h>
#endif

#include <GL\freeglut.h>
#include <glm\glm.hpp>
#include <glm\gtx\string_cast.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <vector>


#include <iterator>
#include <iostream>

#include "Point.h"
#include "AntTweakBar.h"
#include "Fenetrage.h"
#include "Remplissage.h"

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


bool begin_window = FALSE;
bool begin_poly = FALSE;
bool finish_window = FALSE;
bool finish_poly = FALSE;
bool finish_fenetrage = FALSE;
bool move_window = FALSE;

bool fill_polygon = FALSE;

float zoomFactor = 1;

std::vector< Point > points_window;
std::vector< Point > points_poly;
std::vector< Point > points_solution;

Point cursor;

// Menu items
enum MENU_TYPE
{
	MENU_DRAW_POLYGON,
	MENU_DRAW_WINDOW,
	MENU_MOVE,
	MENU_COLORS_POLY,
	MENU_COLORS_WINDOW,
	MENU_REMPLISSAGE,
};

enum COLOR_TYPE
{
	COLOR_RED,
	COLOR_BLUE,
	COLOR_GREEN,
	COLOR_YELLOW,
	COLOR_VIOLET,
	COLOR_ORANGE,
};

int redP = 255, greenP = 0, blueP = 0;
int redW = 0, greenW = 255, blueW = 0;

int width = 640;
int height = 480;

float moveX = 0;
float moveY = 0;

int lastMouseX, lastMouseY;
int lastMoveX, lastMoveY;
int firstMouseX, firstMouseY;

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

	glColor3ub(redW, greenW, blueW);
}



void drawLines(std::vector<Point> points)
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
	
	for (size_t i = 0; i < points.size(); ++i)
	{
		const unsigned int SIZE = 5;
		const Point& pt = points[i];
		glBegin(GL_POINTS);
		glPointSize(10);
		glVertex2i(pt.x, pt.y);
		glEnd();
	}
}

void DrawPoly()
{
	glColor3ub(redP, greenP, blueP);
	drawPoints(points_poly);
	drawLines(points_poly);
}

void DrawWindow()
{
	glColor3ub(redW, greenW, blueW);
	drawPoints(points_window);
	drawLines(points_window);
}

void DrawSolution()
{
	glColor3ub(255, 255, 255);
	drawPoints(points_solution);
	drawLines(points_solution);
}


void display(void)
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-(width/2), (width/2), -(height/2), height/2, -1, 1);
	glScaled(zoomFactor, zoomFactor, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3ub(1, 1, 1);

	if (begin_poly)
	{
		glColor3ub(redP, greenP, blueP);
		drawPoints(points_poly);
	}

	if (finish_poly)
		DrawPoly();

	if (begin_window)
	{
		DrawPoly();
		glColor3ub(redW, greenW, blueW);
		drawPoints(points_window);
	}
		
	if (finish_window)
	{
		DrawPoly();

		moveX = firstMouseX - lastMouseX;
		moveY = firstMouseY - lastMouseY;

		if (moveX != lastMoveX || moveY != lastMoveX)
		{
			if (moveX != lastMoveX)
			{
				for (size_t i = 0; i < points_window.size(); ++i)
					points_window[i].x -= moveX - lastMoveX;
				lastMoveX = moveX;
			}
			if (moveY != lastMoveY)
			{
				for (size_t i = 0; i < points_window.size(); ++i)
					points_window[i].y -= moveY - lastMoveY;
				lastMoveY = moveY;

			}
			std::cout << "Fenetrage" << std::endl;
		    Fenetrage(points_solution, points_window, points_poly, finish_fenetrage);
		}
		//std::cout << "Move " << moveX << " " << moveY << std::endl;
		DrawWindow();
		DrawSolution();
	}
	if (fill_polygon)
	{
		printf("Remplissage\n");
		glColor3ub(redP, greenP, blueP);
		drawPoints(DrawInsidePixel(points_solution, width, height, zoomFactor));
		fill_polygon = FALSE;
	}

/*	if (finish_fenetrage)
	{
		DrawSolution();
		DrawWindow();
	}*/
	
	glutSwapBuffers();

}

// Menu handling function definition
void menu(int item)
{
	switch (item)
	{
		case MENU_DRAW_POLYGON:
		{
			begin_poly = TRUE;
			break;
		}
		case MENU_DRAW_WINDOW:
		{
			begin_window = TRUE;
			finish_poly = FALSE;
			break;
		}

		case MENU_MOVE:
		{
			move_window = true;
			break;
		}

		case MENU_REMPLISSAGE:
		{
			fill_polygon = TRUE;
			break;
		}
		default:
		{

			break;
		}
	}
	glutPostRedisplay();
}
void processColorWindowMenu(int option)
{
	switch (option)
	{
		case COLOR_GREEN:
			red = 0;
			green = 153;
			blue = 0; break;
		case COLOR_BLUE:
			red = 0;
			green = 0;
			blue = 204; break;
		case COLOR_VIOLET:
			red = 153;
			green = 0;
			blue = 153; break;
		
		default:
		{
			break;
		}
	}
	redW = red;
	greenW = green;
	blueW = blue;
}

void processColorPolyMenu(int option)
{
	switch (option) 
	{
		case COLOR_RED:
			red = 204;
			green = 0;
			blue = 0; break;
		case COLOR_ORANGE:
			red = 255;
			green = 128;
			blue = 0; break;
		case COLOR_YELLOW:
			red = 255;
			green = 255;
			blue = 51; break;

		default:
		{
			break;
		}
	}
	redP = red;
	greenP = green;
	blueP = blue;
}

void mouseZoom(int button, int dir, int x, int y)
{
	if (dir > 0)
		zoomFactor -= 0.05f;
	else
		zoomFactor += 0.05f;

	if (zoomFactor < 0.5)
		zoomFactor = 0.5;
	else if (zoomFactor > 2)
		zoomFactor = 2;
	std::cout << zoomFactor << std::endl;
	glutPostRedisplay();
}


void mouseMove(int x, int y)
{
	if (move_window)
	{
		y = height - y;
		x = x - (width / 2);
		y = y - (height / 2);

		x = x / zoomFactor;
		y = y / zoomFactor;
		cursor = Point(x, y);

		lastMouseX = x;
		lastMouseY = y;
	}
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	y = height - y;
	x = x - (width / 2);
	y = y - (height / 2);

	x = x / zoomFactor;
	y = y / zoomFactor;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		if (begin_window)
			points_window.push_back(Point(x, y));
		if (begin_poly)
			points_poly.push_back(Point(x, y));
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		firstMouseX = x;
		firstMouseY = y;
		lastMoveX = firstMouseX - lastMouseX;
		lastMoveY = firstMouseY - lastMouseY;

		std::cout << "LastMove " << lastMoveX << "," << lastMoveY << std::endl;
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
		if (begin_poly)
		{
			begin_poly = FALSE;
			finish_poly = TRUE;
		}
		if (begin_window)
		{
			begin_window = FALSE;
			finish_window = TRUE;
		}

		break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutCreateWindow("OpenGL");

	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutMouseWheelFunc(mouseZoom);
	glutMotionFunc(mouseMove);

	int colorPolyMenu = glutCreateMenu(processColorPolyMenu);
	glutAddMenuEntry("Red", COLOR_RED);
	glutAddMenuEntry("Orange", COLOR_ORANGE);
	glutAddMenuEntry("Yellow", COLOR_YELLOW);

	int colorWindowMenu = glutCreateMenu(processColorWindowMenu);
	glutAddMenuEntry("Green", COLOR_GREEN);
	glutAddMenuEntry("Blue", COLOR_BLUE);
	glutAddMenuEntry("Violet", COLOR_VIOLET);

	glutCreateMenu(menu);
	glutAddSubMenu("Colors for Poly", colorPolyMenu);
	glutAddSubMenu("Colors for Window", colorWindowMenu);
	glutAddMenuEntry("Draw polygon", MENU_DRAW_POLYGON);
	glutAddMenuEntry("Draw window", MENU_DRAW_WINDOW);
	glutAddMenuEntry("Move window", MENU_MOVE);
	glutAddMenuEntry("Remplissage", MENU_REMPLISSAGE);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
	return 0;
}

