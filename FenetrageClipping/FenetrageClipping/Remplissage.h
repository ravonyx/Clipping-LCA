#pragma once

#include "Point.h"
#include <glm\glm.hpp>
#include <vector>
#include <algorithm>
#include "Fenetrage.h"
#include "triangle.h"

std::vector<Point> DrawInsidePixel(std::vector<Point> &PolygonPoint,int width,int height,float zoomFactor);
bool PointInTriangle(Point pt, Point v1, Point v2, Point v3);
float sign(Point p1, Point p2, Point p3);