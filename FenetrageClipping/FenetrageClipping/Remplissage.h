#pragma once

#include "Point.h"
#include <glm\glm.hpp>
#include <vector>
#include <algorithm>
#include "Windowing.h"
#include "triangle.h"

std::vector<Point> DrawInsidePixel(std::vector<Point> &PolygonPoint,int width,int height,float zoomFactor);
std::vector<Triangle> getTriangleInPoly(std::vector<Point> &PolygonPoint);
std::vector<Point> getSurroundRect(std::vector<Point> &PolygonPoint);
bool PointInTriangle(Point pt, Point v1, Point v2, Point v3);
float sign(Point p1, Point p2, Point p3);

int isEdgeIntersect(int n, const std::vector<Point> &p);
int isVertexInsideNewPoly(int n, const std::vector<Point> &p);
int isVertexEar(int n, const std::vector<Point> &p);