#include "Remplissage.h"
std::vector<Point> DrawInsidePixel(std::vector<Point> &PolygonPoint, int width, int height, float zoomFactor)
{
	std::vector<Point> InsidePoints;
	if (PolygonPoint.size() < 3)
		return InsidePoints;

	//bool isInside(test, &PolygonPoint);
	//bool inside = PointInTriangle(test, PolygonPoint[0], PolygonPoint[1], PolygonPoint[2]);

	

	std::vector<Triangle> PolygonTriangle;

	PolygonTriangle = getTriangleInPoly(PolygonPoint);


	float x = 0;
	float y = 0;

	std::vector<Point> rect = getSurroundRect(PolygonPoint);
	int m_height = rect[1].y - rect[0].y;
	int m_width = rect[1].x - rect[0].x;

	for (int a = rect[0].y; a < rect[1].y; a++)
	{
		for (int b = rect[0].x; b < rect[1].x; b++)
		{
			for (int k = 0; k < PolygonTriangle.size(); k++)
				if (PointInTriangle(Point(b, a), PolygonTriangle[k].A, PolygonTriangle[k].B, PolygonTriangle[k].C))
				{
					InsidePoints.push_back(Point(b, a));

					//Point(x, y).print();

				}
		}
	}
	return InsidePoints;
}

std::vector<Triangle> getTriangleInPoly(std::vector<Point>& PolygonPoint)
{
	std::vector<Triangle> PolygonTriangle;
	if (PolygonPoint.size() < 3)
		return PolygonTriangle;

	Point triangleOrigin;
	triangleOrigin = PolygonPoint[0];

	Point secondPoint = PolygonPoint[1];

	Point lastTop = PolygonPoint[2];
	PolygonTriangle.push_back(Triangle(triangleOrigin, secondPoint, lastTop));
	int step = 0;
	for (int j = 3; j < PolygonPoint.size(); j++)
	{
		PolygonTriangle.push_back(Triangle(triangleOrigin, lastTop, PolygonPoint[j]));
		lastTop = PolygonPoint[j];

	}

	return PolygonTriangle;
}

std::vector<Point> getSurroundRect(std::vector<Point>& PolygonPoint)
{
	
	Point upLeft = PolygonPoint[0];
	Point downRight = PolygonPoint[0];
	std::vector<Point> corners;
	//Point top
	for (int i = 0; i < PolygonPoint.size(); i++)
	{
		if (PolygonPoint[i].x > downRight.x)
			downRight.x = PolygonPoint[i].x;
		if (PolygonPoint[i].y > downRight.y)
			downRight.y = PolygonPoint[i].y;
		if (PolygonPoint[i].x < upLeft.x )
			upLeft.x = PolygonPoint[i].x;
		if (PolygonPoint[i].y < upLeft.y)
			upLeft.y = PolygonPoint[i].y;

	}
	corners.push_back(upLeft);
	corners.push_back(downRight);

	return corners;
}

bool PointInTriangle(Point pt, Point v1, Point v2, Point v3)
{
	bool b1, b2, b3;

	b1 = sign(pt, v1, v2) < 0.0f;
	b2 = sign(pt, v2, v3) < 0.0f;
	b3 = sign(pt, v3, v1) < 0.0f;

	return ((b1 == b2) && (b2 == b3));
}

float sign(Point p1, Point p2, Point p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}
