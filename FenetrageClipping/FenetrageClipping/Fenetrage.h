#pragma once

#include "Point.h"
#include <glm\glm.hpp>
#include <vector>
#include <algorithm>

float isVisble(Point p1, Point p2, Point p_to_test);
bool isInside(Point point, std::vector< Point > &points_window);
bool cut(Point p1, Point p2, Point p3, Point p4, glm::vec2 &result);
void Fenetrage(std::vector< Point > &points_solution, std::vector< Point > &points_window, std::vector< Point > points_poly, bool &finish_fenetrage);