#include "Remplissage.h"
std::vector<Point> DrawInsidePixel(std::vector<Point> &PolygonPoint, int width, int height, float zoomFactor)
{



	//bool isInside(test, &PolygonPoint);
	//bool inside = PointInTriangle(test, PolygonPoint[0], PolygonPoint[1], PolygonPoint[2]);

	std::vector<Point> InsidePoints;
	std::vector<Triangle> PolygonTriangle;
	if (PolygonPoint.size() < 3)
		return InsidePoints;

	Point triangleOrigin;
	triangleOrigin = PolygonPoint[0];

	Point secondPoint = PolygonPoint[1];

	Point lastTop = PolygonPoint[2];
	PolygonTriangle.push_back(Triangle(triangleOrigin, secondPoint ,lastTop));
	int step = 0;
	for (int j = 3; j < PolygonPoint.size(); j++)
	{
		PolygonTriangle.push_back(Triangle(triangleOrigin, lastTop, PolygonPoint[j]));
		lastTop = PolygonPoint[j];

	}


	float x = 0;
	float y = 0;
	for (int i = 0; i < width * height; i++)
	{
		if (i % width == 0 && i > width)
			y++;

		x = i % width;

		y = height - y;
		x = x - (width / 2);
		y = y - (height / 2);

		//x = x / zoomFactor;
		//y = y / zoomFactor;

		for (int k = 0; k < PolygonTriangle.size(); k++)
			if (PointInTriangle(Point(x, y), PolygonTriangle[k].A , PolygonTriangle[k].B, PolygonTriangle[k].C))
			{
				InsidePoints.push_back(Point(x, y));
				//Point(x, y).print();

			}
	}
	//bool inside = PointInTriangle(test, Point(0,0), Point(200,0), Point(0,200));
	//if (inside)
	//	printf("Inside\n");
	return InsidePoints;
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
