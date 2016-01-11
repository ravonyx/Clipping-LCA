#pragma once
#include "Point.h"
#include <vector>
#include <GL\glew.h>
#define _USE_MATH_DEFINES
#include <math.h>

void drawLines(const std::vector<Point> &points);
void drawCircle(int x, int y, int nb_circle_points, int radius, float angleStart = 0.0f, float angleEnd = 360.0f);
void drawPoints(const std::vector< Point > &points, float size = 1);
void DrawPoly(const std::vector< Point > points_poly, int red, int green, int blue);
void DrawWindow(const std::vector< Point > points_window, int red, int green, int blue);
void DrawSolution(const std::vector< Point > points_solution);