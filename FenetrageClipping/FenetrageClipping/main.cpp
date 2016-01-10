#include "DrawElements.h"

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
#include <iterator>
#include <iostream>

#include "AntTweakBar.h"
#include "Fenetrage.h"
#include "Interfaces.h"

extern s_display_manager g_display_manager;
#include "Remplissage.h"

bool fill_polygon = false;
float zoomFactor = 1;

std::vector< Point > points_window;
std::vector< Point > points_poly;
std::vector< Point > points_solution;

Point center;
int moveRadius, radius;

int redP = 255, greenP = 0, blueP = 0;
int redW = 0, greenW = 255, blueW = 0;

int width = 640;
int height = 480;

int lastMouseX, lastMouseY;
int firstMouseX, firstMouseY;

float moveX = 0;
float moveY = 0;
int lastMoveX, lastMoveY;

float moveXCircle = 0;
int lastMoveXCircle;

void display(void);
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

// Menu handling function definition
void menu(int item)
{
	switch (item)
	{
		case MENU_DRAW_POLYGON:
		{
			g_display_manager.begin_poly = true;
			break;
		}
		case MENU_DRAW_WINDOW:
		{
			g_display_manager.begin_window = true;
			g_display_manager.finish_poly = false;
			break;
		}

		case MENU_MOVE:
		{
			g_display_manager.move_window = true;
			break;
		}
		case MENU_DRAW_CIRCLE:
		{
			g_display_manager.begin_circle = true;
		}

		case MENU_REMPLISSAGE:
		{
			fill_polygon = true;
			break;
		}

		default:
		{
			break;
		}
	}
	
	glutSwapBuffers();
}


void processColorWindowMenu(int option)
{
	switch (option)
	{
	case COLOR_GREEN:
		redW = 0;
		greenW = 153;
		blueW = 0; break;
	case COLOR_BLUE:
		redW = 0;
		greenW = 0;
		blueW = 204; break;
	case COLOR_VIOLET:
		redW = 153;
		greenW = 0;
		blueW = 153; break;

	default:
	{
		break;
	}
	}
}
void processColorPolyMenu(int option)
{
	switch (option)
	{
	case COLOR_RED:
		redP = 204;
		greenP = 0;
		blueP = 0; break;
	case COLOR_ORANGE:
		redP = 255;
		greenP = 128;
		blueP = 0; break;
	case COLOR_YELLOW:
		redP = 255;
		greenP = 255;
		blueP = 51; break;

	default:
	{
		break;
	}
	}
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
	if (g_display_manager.move_window || g_display_manager.begin_circle)
	{
		y = height - y;
		x = x - (width / 2);
		y = y - (height / 2);

		x = x / zoomFactor;
		y = y / zoomFactor;

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
		if (g_display_manager.begin_window)
			points_window.push_back(Point(x, y));
		if (g_display_manager.begin_poly)
			points_poly.push_back(Point(x, y));
		if (g_display_manager.begin_circle)
		{
			radius = moveRadius;
			g_display_manager.finish_circle = true;
			g_display_manager.begin_circle = false;
		}

	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		firstMouseX = x;
		firstMouseY = y;
		lastMoveX = firstMouseX - lastMouseX;
		lastMoveY = firstMouseY - lastMouseY;

		if (g_display_manager.begin_circle)
		{
			center = Point(x, y);
			firstMouseX = x;
			firstMouseY = y;
			lastMoveXCircle = firstMouseX - lastMouseX;
		}
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: exit(0); break;
	case 32:
		if (g_display_manager.begin_poly)
		{
			g_display_manager.begin_poly = false;
			g_display_manager.finish_poly = true;
		}
		if (g_display_manager.begin_window)
		{
			g_display_manager.begin_window = false;
			g_display_manager.finish_window = true;
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
	glutAddMenuEntry("Draw circle", MENU_DRAW_CIRCLE);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
	return 0;
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

	if (g_display_manager.begin_poly)
	{
		glColor3ub(redP, greenP, blueP);
		drawPoints(points_poly);
	}

	if (g_display_manager.finish_poly)
		DrawPoly(points_poly, redP, greenP, blueP);

	if (g_display_manager.begin_window)
	{
		DrawPoly(points_poly, redP, greenP, blueP);
		glColor3ub(redW, greenW, blueW);
		drawPoints(points_window);
	}
		
	if (g_display_manager.finish_window)
	{
		DrawPoly(points_poly, redP, greenP, blueP);

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
		    Fenetrage(points_solution, points_window, points_poly, g_display_manager.finish_fenetrage);
		}

		DrawWindow(points_window, redW, greenW, blueW);
		DrawSolution(points_solution);
	}
	if (g_display_manager.begin_circle)
	{
		moveRadius = 0;
		moveXCircle = center.x - lastMouseX;
		if (moveXCircle != lastMoveXCircle)
		{
			lastMoveXCircle = moveXCircle;
			moveRadius += abs(moveXCircle);
		}
		if(moveRadius > 0)
			drawCircle(center.x, center.y, 100, moveRadius);
	}

	if (fill_polygon)
	{
		printf("Remplissage\n");
		glColor3ub(redP, greenP, blueP);
		drawPoints(DrawInsidePixel(points_solution, width, height, zoomFactor));
		//fill_polygon = false;
	}

	if (g_display_manager.finish_circle)
		drawCircle(center.x, center.y, 100, radius);
		
	glutSwapBuffers();

}
