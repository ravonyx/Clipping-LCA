#include "DrawElements.h"

void draw_lines(const std::vector<Point> &points)
{
	glBegin(GL_LINE_LOOP);
	for (size_t i = 0; i < points.size(); ++i)
		glVertex3f(points[i].x, points[i].y, 0);
	glEnd();
}
void draw_points(const std::vector< Point > &points, float size)
{
	glPointSize(size);
	glBegin(GL_POINTS);
	for (size_t i = 0; i < points.size(); ++i)
	{
		glVertex2i(points[i].x, points[i].y);
	}
	glEnd();
}

void draw_circle(int x, int y, int nb_circle_points, int radius, float angleStart /*= 0.0f*/, float angleEnd  /*= 360.0f*/)
{
	glBegin(GL_LINE_LOOP);

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

void draw_poly(const std::vector< Point > points_poly, int red, int green, int blue)
{
	glColor3ub(red, green, blue);
	draw_points(points_poly);
	draw_lines(points_poly);
}
void draw_window(const std::vector< Point > points_window, int red, int green, int blue)
{
	glColor3ub(red, green, blue);
	draw_points(points_window);
	draw_lines(points_window);
}
void draw_solution(const std::vector< Point > points_solution)
{
	glColor3ub(255, 255, 255);
	draw_points(points_solution);
	draw_lines(points_solution);
}