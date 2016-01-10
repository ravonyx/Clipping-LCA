#pragma once

#include "Point.h"
#include <glm\glm.hpp>
#include <vector>
#include <algorithm>

float isVisble(Point p1, Point p2, Point p_to_test);
bool isInside(Point point, const std::vector< Point > &points_window);
bool cut(Point p1, Point p2, Point p3, Point p4, glm::vec2 &result);
void add_intersection(Point p1, Point p2, std::vector< Point > &points_solution, float t);
void Fenetrage(std::vector< Point > &points_solution, const std::vector< Point > &points_window, const std::vector< Point > &points_poly, bool &finish_fenetrage);