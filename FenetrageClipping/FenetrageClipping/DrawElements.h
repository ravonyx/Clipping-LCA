#pragma once
#include "Point.h"
#include <vector>
#include <GL\glew.h>
#define _USE_MATH_DEFINES
#include <math.h>

void draw_lines(const std::vector<Point> &points);
void draw_circle(int x, int y, int nb_circle_points, int radius, float angleStart = 0.0f, float angleEnd = 360.0f);
void draw_points(const std::vector< Point > &points, float size = 1);
void draw_poly(const std::vector< Point > points_poly, int red, int green, int blue);
void draw_window(const std::vector< Point > points_window, int red, int green, int blue);
void draw_solution(const std::vector< Point > points_solution);