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
	std::vector<Point> m_PolygonPoint = PolygonPoint;
	if (PolygonPoint.size() < 3)
		return PolygonTriangle;
	

	/*PolygonTriangle.push_back(Triangle(m_PolygonPoint[0], m_PolygonPoint[1], m_PolygonPoint[2]));
	
	for (int j = 3; j < m_PolygonPoint.size() - 1; j++)
	{
		if (isVertexEar(j, m_PolygonPoint))
		{
			PolygonTriangle.push_back(Triangle(m_PolygonPoint[j-1], m_PolygonPoint[j], m_PolygonPoint[j+1]));
		}
		//PolygonTriangle.push_back(Triangle(triangleOrigin, lastTop, PolygonPoint[j]));
		//lastTop = PolygonPoint[j];

	}*/

	for (size_t t = m_PolygonPoint.size() - 1, i = 0, j = 1; i < m_PolygonPoint.size(); t = i++, j = (i + 1) % m_PolygonPoint.size())
	{
		if (m_PolygonPoint.size() == 3)
		{
			PolygonTriangle.push_back(Triangle(m_PolygonPoint[0], m_PolygonPoint[1], m_PolygonPoint[2]));

			break;
		}

		if (isVertexEar(i, m_PolygonPoint))
		{
			PolygonTriangle.push_back(Triangle(m_PolygonPoint[t], m_PolygonPoint[i], m_PolygonPoint[j]));

			m_PolygonPoint.erase(m_PolygonPoint.begin() + i, m_PolygonPoint.begin() + i + 1);

			t = m_PolygonPoint.size() - 1;
			i = 0;
			j = 1;
		}
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
		if (PolygonPoint[i].x < upLeft.x)
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

int isEdgeIntersect(int n, const std::vector<Point> &p)
{
	Point v = p[n];
	std::vector<Point> a;

	for (size_t i = 0; i < p.size(); i++)
		if (i != n)
			a.push_back(p[i]);

	int c = 0, cnt = a.size(), prev = (cnt + (n - 1)) % cnt, next = n % cnt;

	Point v1 = a[prev], v2 = a[next];

	for (size_t i = 0, j = cnt - 1; i < cnt; j = i++)
	{
		if (prev == i || prev == j || next == i || next == j)
			continue;

		Point v4 = a[j], v3 = a[i];

		float denominator = ((v4.y - v3.y) * (v2.x - v1.x)) - ((v4.x - v3.x) * (v2.y - v1.y));

		if (!denominator)
			continue;

		float ua = (((v4.x - v3.x) * (v1.y - v3.y)) - ((v4.y - v3.y) * (v1.x - v3.x))) / denominator;
		float ub = (((v2.x - v1.x) * (v1.y - v3.y)) - ((v2.y - v1.y) * (v1.x - v3.x))) / denominator;

		//float x = v1.x + (ua * (v2.x - v1.x)), y = v1.y + (ua * (v2.y - v1.y));

		if (ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1)
		{
			c = 1;
			break;
		}
	}
	return c;
}

int isVertexInsideNewPoly(int n, const std::vector<Point> &p)
{
	Point v = p[n];
	std::vector<Point> a;

	for (size_t i = 0; i < p.size(); i++)
		if (i != n)
			a.push_back(p[i]);

	int c = 1;

	for (size_t i = 0, j = a.size() - 1; i < a.size(); j = i++)
	{
		if ((((a[i].y <= v.y) && (v.y < a[j].y)) || ((a[j].y <= v.y) && (v.y < a[i].y))) && (v.x >(a[j].x - a[i].x) * (v.y - a[i].y) / (a[j].y - a[i].y) + a[i].x))
			c = !c;
	}

	return c;
}

int isVertexEar(int n, const std::vector<Point> &p)
{
	return (isVertexInsideNewPoly(n, p) && !isEdgeIntersect(n, p));
}
