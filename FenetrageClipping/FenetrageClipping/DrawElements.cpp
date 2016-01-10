#include "DrawElements.h"

void drawLines(const std::vector<Point> &points)
{
	glBegin(GL_LINE_LOOP);
	for (size_t i = 0; i < points.size(); ++i)
		glVertex3f(points[i].x, points[i].y, 0);
	glEnd();
}
void drawPoints(const std::vector< Point > &points)
{
	glBegin(GL_POINTS);
	for (size_t i = 0; i < points.size(); ++i)
	{
		
		glPointSize(10);
		glVertex2i(points[i].x, points[i].y);
	
	}
	glEnd();
}

/*void drawCircle(int x, int y, int nb_circle_points, int radius)
{
	glColor3ub(0, 0, 255);
	glBegin(GL_LINE_LOOP);
	float angle;

	for (size_t i = 0; i < nb_circle_points; ++i)
	{
		angle = 0.5 * M_PI *i / nb_circle_points;
		glVertex2f(x + cos(angle)*radius, y + sin(angle)*radius);
	}
	glEnd();
}*/

void drawCircle(int x, int y, int nb_circle_points, int radius, float angleStart /*= 0.0f*/, float angleEnd  /*= 360.0f*/)
{
	glColor3ub(0, 0, 255);
	glBegin(GL_LINE_LOOP);
	if (angleStart != 0.0f || angleEnd != 360.0f)
		glVertex2f(x, y);

	float angle;
	float angleOffset = angleEnd - angleStart;
	if(angleStart > angleEnd)
		angleOffset = (angleEnd - angleStart) + 360.0;
	for (size_t i = 0; i < nb_circle_points; ++i)
	{
		angle = (angleStart / 360.0) * M_PI * 2 + (angleOffset / 360.0) * 2 * M_PI * i / nb_circle_points;
		glVertex2f(x + cos(angle)*radius, y + sin(angle)*radius);
	}
	glEnd();
}

void DrawPoly(const std::vector< Point > points_poly, int red, int green, int blue)
{
	glColor3ub(red, green, blue);
	drawPoints(points_poly);
	drawLines(points_poly);
}
void DrawWindow(const std::vector< Point > points_window, int red, int green, int blue)
{
	glColor3ub(red, green, blue);
	drawPoints(points_window);
	drawLines(points_window);
}
void DrawSolution(const std::vector< Point > points_solution)
{
	glColor3ub(255, 255, 255);
	drawPoints(points_solution);
	drawLines(points_solution);
}