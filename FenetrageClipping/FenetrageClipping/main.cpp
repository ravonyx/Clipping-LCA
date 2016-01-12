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
#include "Windowing.h"
#include "Interfaces.h"

extern s_display_manager g_display_manager;
#include "Remplissage.h"

bool fill_polygon = false;
float zoom_factor = 1;

std::vector< std::vector<Point> > all_points_poly;
std::vector< std::vector<Point> > all_points_solutions;
std::vector< std::vector<Point> > all_points_window;

std::vector< Point > points_window;
std::vector< Point > points_poly;
std::vector< Point > points_solution;

Point center;
int move_radius, radius;

int redP = 255, greenP = 0, blueP = 0;
int redW = 0, greenW = 255, blueW = 0;

int width = 640;
int height = 480;

int last_mousex, last_mousey;
int first_mousex, first_mousey;

float moveX = 0;
float moveY = 0;
int last_movex, last_movey;

float move_circlex = 0;
int last_move_circlex;

int index_move_win;

void display(void);
void Initialize()
{
	printf("Version OpenGL : %s\n", glGetString(GL_VERSION));
	printf("Fabricant : %s\n", glGetString(GL_VENDOR));
	printf("Pilote : %s\n", glGetString(GL_RENDERER));
	printf("Version GLSL : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glewInit();
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
			break;
		}
		case MENU_REMPLISSAGE:
		{
			fill_polygon = true;
			break;
		}
		case MENU_DRAW_CIRCLE:
		{
			g_display_manager.begin_circle = true;
		}
		case MENU_MOVE:
		{
			g_display_manager.click_window = true;
			break;
		}
		case MENU_QUIT_MOVE:
		{
			g_display_manager.click_window = false;
			g_display_manager.move_window = false;
			break;
		}
		default:
		{
			break;
		}
	}
	glutPostRedisplay();
}

//Colors Menu
void processColorWindowMenu(int option)
{
	switch (option)
	{
		case COLOR_GREEN:
			redW = 0;
			greenW = 153;
			blueW = 0; break;
		case COLOR_BLUE:
			redW = 102;
			greenW = 255;
			blueW = 255; break;
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
		zoom_factor -= 0.05f;
	else
		zoom_factor += 0.05f;
	if (zoom_factor < 0.5)
		zoom_factor = 0.5;
	else if (zoom_factor > 2)
		zoom_factor = 2;
	glutPostRedisplay();
}
void mouseMove(int x, int y)
{
	if (g_display_manager.move_window || g_display_manager.begin_circle)
	{
		y = height - y;
		x = x - (width / 2);
		y = y - (height / 2);

		x = x / zoom_factor;
		y = y / zoom_factor;

		last_mousex = x;
		last_mousey = y;
	}
	glutPostRedisplay();
}
void mouse(int button, int state, int x, int y)
{
	y = height - y;
	x = x - (width / 2);
	y = y - (height / 2);

	x = x / zoom_factor;
	y = y / zoom_factor;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		//Add point to poly and window
		if (g_display_manager.begin_window)
			points_window.push_back(Point(x, y));
		if (g_display_manager.begin_poly)
			points_poly.push_back(Point(x, y));

		//Fix radius
		if (g_display_manager.begin_circle)
		{
			radius = move_radius;
			g_display_manager.finish_circle = true;
			g_display_manager.begin_circle = false;
		}

	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//For move window 
		if (g_display_manager.click_window)
		{
			for (int i = 0; i < all_points_window.size(); i++)
			{
				//check if click is inside a window
				if (is_inside(Point(x, y), all_points_window[i]))
				{
					g_display_manager.move_window = true;
					index_move_win = i; 

					first_mousex = x;
					first_mousey = y;
					last_mousex = x;
					last_mousey = y;

					last_movex = 0;
					last_movey = 0;
				}
			}
		}

		//Set center and position of circle
		if (g_display_manager.begin_circle)
		{
			center = Point(x, y);
			first_mousex = x;
			first_mousey = y;
			last_move_circlex = first_mousex - last_mousex;
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
		//Add poly to list of poly and clear
		if (g_display_manager.begin_poly)
		{
			g_display_manager.begin_poly = false;
			all_points_poly.push_back(points_poly);
			points_poly.clear();
		}
		//Add window to list of windows and clear
		if (g_display_manager.begin_window)
		{
			g_display_manager.begin_window = false;
			all_points_window.push_back(points_window);
			points_window.clear();
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

	//Callback functions
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutMouseWheelFunc(mouseZoom);
	glutMotionFunc(mouseMove);

	//Create Menu
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
	glutAddMenuEntry("Remplissage", MENU_REMPLISSAGE);
	glutAddMenuEntry("Draw circle", MENU_DRAW_CIRCLE);
	glutAddMenuEntry("Move window", MENU_MOVE);
	glutAddMenuEntry("Quit Move window", MENU_QUIT_MOVE);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
	return 0;
}

void display(void)
{
	//Clear screen
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	//Set view and zoom
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-(width/2), (width/2), -(height/2), height/2, -1, 1);
	glScaled(zoom_factor, zoom_factor, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Color by default
	glColor3ub(1, 1, 1);

	//Draw points of poly
	if (g_display_manager.begin_poly)
	{
		glColor3ub(redP, greenP, blueP);
		draw_points(points_poly);
	}
	//Draw points of window
	if (g_display_manager.begin_window)
	{
		glColor3ub(redW, greenW, blueW);
		draw_points(points_window);
	}
	
	//Retrieve last move if in mode move window
	if (g_display_manager.move_window)
	{
		moveX = first_mousex - last_mousex;
		moveY = first_mousey - last_mousey;
		if ((moveX != last_movex || moveY != last_movey))
		{
			//move the window
			if (moveX != last_movex)
			{
				for (size_t i = 0; i < all_points_window[index_move_win].size(); ++i)
					all_points_window[index_move_win][i].x -= moveX - last_movex;
				last_movex = moveX;
			}
			if (moveY != last_movey)
			{
				for (size_t i = 0; i < all_points_window[index_move_win].size(); ++i)
					all_points_window[index_move_win][i].y -= moveY - last_movey;
				last_movey = moveY;

			}
		}
	}

	//Windowing
	if (all_points_poly.size() >= 1 && all_points_window.size() >= 1)
	{
		all_points_solutions.clear();
		for (int i = 0; i < all_points_poly.size(); i++)
		{
			for (int j = 0; j < all_points_window.size(); j++)
			{
				windowing(points_solution, all_points_window[j], all_points_poly[i]);
				all_points_solutions.push_back(points_solution);
			}
				
		}
	}

	//Ajust the radius of circle
	if (g_display_manager.finish_circle)
	{
		moveX = first_mousex - last_mousex;
		moveY = first_mousey - last_mousey;

		std::vector<Point> intersectPoints;
		float angle1 = 0, angle2 = 0;

		if (all_points_window.size() >= 1)
		{
			for (int i = 0; i < all_points_window.size(); i++)
				circle_windowing(center, radius, all_points_window[i], intersectPoints, angle1, angle2);

		}	
		glColor3ub(255, 0, 0);
		draw_points(intersectPoints, 5);
		glColor3ub(0, 0, 204);
		draw_circle(center.x, center.y, 500, radius);
	}
	
	//Draw all poly
	if (all_points_poly.size() >= 1)
	{
		for (int i = 0; i < all_points_poly.size(); i++)
			draw_poly(all_points_poly[i], redP, greenP, blueP);
	}

	//Draw all window
	if (all_points_window.size() >= 1)
	{
		for (int i = 0; i < all_points_window.size(); i++)
			draw_window(all_points_window[i], redW, greenW, blueW);
	}

	//Draw solutions
	if (all_points_solutions.size() >= 1)
	{
		for (int i = 0; i < all_points_solutions.size(); i++)
			draw_solution(all_points_solutions[i]);
	}

	//Ajust the radius of circle
	if (g_display_manager.begin_circle)
	{
		move_radius = 5;
		move_circlex = center.x - last_mousex;
		if (move_circlex != last_move_circlex)
		{
			last_move_circlex = move_circlex;
			move_radius += abs(move_circlex);
		}
		glColor3ub(0, 0, 204);
		if(move_radius > 0)
			draw_circle(center.x, center.y, 500, move_radius);
	}

	//Remplissage
	if (fill_polygon)
	{
		glColor3ub(redP, greenP, blueP);

		for (size_t i = 0; i < all_points_solutions.size(); i++)
		{
			std::vector <Point> inside_points = DrawInsidePixel(all_points_solutions[i], width, height, zoom_factor);
			draw_points(inside_points);
		}
	}

	glutSwapBuffers();
}
