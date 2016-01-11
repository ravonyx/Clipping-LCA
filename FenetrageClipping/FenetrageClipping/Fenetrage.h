#pragma once

#include "Point.h"
#include <glm\glm.hpp>
#include <vector>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>


float isVisble(Point p1, Point p2, Point p_to_test);
bool isInside(Point point, const std::vector< Point > &points_window);
bool cut(Point p1, Point p2, Point p3, Point p4, glm::vec2 &result);
void add_intersection(Point p1, Point p2, std::vector< Point > &points_solution, float t);
void Fenetrage(std::vector< Point > &points_solution, const std::vector< Point > &points_window, const std::vector< Point > &points_poly, bool &finish_fenetrage);
int circle_intersection(const Point& circleCenter, float cirlceRadius, const Point& lineStart, const Point& lineEnd, std::vector<Point>& intersecPoints);
void circle_windowing(const Point& circleCenter, float circleRadius, std::vector<Point>& points_window, std::vector<Point>& intersectPoints, float &angleDegr1, float &angleDegr2);
float square_distance(Point a, Point b);
bool is_between(Point a, Point c, Point b);