#include "Windowing.h"

float is_visble(Point p1, Point p2, Point p_to_test)
{
	glm::vec2 AB = { p2.x - p1.x, p2.y - p1.y };

	//normalize the vector
	float length = sqrtf(AB.x * AB.x + AB.y * AB.y);
	AB /= length;

	//create normal interior
	glm::vec2 normal_int(-AB.y, AB.x);
	glm::vec2 droite(p1.x - p_to_test.x, p1.y - p_to_test.y);

	float result = glm::dot(normal_int, droite);
	return result;
}

bool is_inside(Point point, const std::vector< Point > &points)
{
	bool inside = false;
	size_t i, j;
	for (i = 0, j = points.size() - 1; i < points.size(); j = i++)
	{
		if (((points[i].y >= point.y) != (points[j].y >= point.y)) && (point.x <= (points[j].x - points[i].x) * (point.y - points[i].y) / (points[j].y - points[i].y) + points[i].x))
			inside = !inside;
	}
	return inside;
}

bool cut(Point p1, Point p2, Point p3, Point p4, glm::vec2 &result)
{
	glm::mat2 intersect = glm::mat2(p2.x - p1.x , p2.y - p1.y, p3.x - p4.x,  p3.y - p4.y);
	glm::mat2 intersectInverse = glm::inverse(intersect);
	float determinant = glm::determinant(intersect);
	if (determinant == 0)
		std::cout << "Matrice non inversible" << std::endl;

	glm::vec2 vec = { p3.x - p1.x, p3.y - p1.y };
	result = intersectInverse * vec;

	float t = result.x;
	float s = result.y;

	bool is_cut = false;
	const bool tIn = t > 0 && t < 1;
	const bool sIn = s > 0 && s < 1;

	//check if values are between 0 & 1
	if ((sIn && tIn) || (!sIn && tIn))
		is_cut = true;
	else
		is_cut = false;
	return is_cut;
}

void add_intersection(Point p1, Point p2, std::vector< Point > &points_solution, float t)
{
	//compute the intersection from the value of t
	glm::vec2 AB = { p2.x - p1.x, p2.y - p1.y };
	glm::vec2 part = AB * t;
	glm::vec2 intersect = { p1.x + part.x, p1.y + part.y };
	points_solution.push_back(Point(intersect.x, intersect.y));
}

void windowing(std::vector< Point > &points_solution, const std::vector< Point > &points_window, const std::vector< Point > &points_poly)
{
	//Add first position to the end 
	std::vector<Point> points_win_copy = points_window;
	points_win_copy.push_back(points_window[0]);
	size_t nb_points_window = points_win_copy.size();

	std::vector<Point> points_poly_copy = points_poly;
	size_t nb_points_poly = points_poly.size();

	int nb_points_solutions;

	nb_points_solutions = 0;

	Point prev_point_poly;
	Point first_point_poly;
	int last_index_poly;

	for (size_t j = 0; j < nb_points_window - 1; j++)
	{
		points_solution.clear();
		nb_points_solutions = 0;

		Point p3 = points_win_copy[j];
		Point p4 = points_win_copy[j + 1];

		for (size_t i = 0; i < nb_points_poly; i++)
		{
			Point current_point_poly = points_poly_copy[i];
			//save first point of poly
			if (i == 0)
				first_point_poly = current_point_poly;
			else
			{
				glm::vec2 res1;
				//check intersection
				if (cut(prev_point_poly, current_point_poly, p3, p4, res1))
				{
					add_intersection(prev_point_poly, current_point_poly, points_solution, res1.x);
					nb_points_solutions++;
				}
			}
			last_index_poly = i;
			prev_point_poly = current_point_poly;
			//if visible - add in solution
			if (is_visble(p3, p4, prev_point_poly) > 0)
			{
				points_solution.push_back(prev_point_poly);
				nb_points_solutions++;
			}
		}
		if (nb_points_solutions > 0)
		{
			glm::vec2 res2;
			//cut fot the last line of poly 
			if (cut(prev_point_poly, first_point_poly, p3, p4, res2))
			{
				add_intersection(prev_point_poly, first_point_poly, points_solution, res2.x);
				nb_points_solutions++;
			}
		}
		points_poly_copy = points_solution;
		nb_points_poly = nb_points_solutions;
	}
}

void circle_windowing(const Point& circle_center, float circle_radius, std::vector<Point>& points_window, std::vector<Point>& points_solutions, float &angleDegr1, float &angleDegr2)
{

	std::cout << "Circle" << std::endl;
	int count = 0;

	std::vector<Point> intersectPoints;
	std::vector<Point> points_window_copy = points_window;
	points_window_copy.push_back(points_window_copy[0]);

	int found = 0;
	for (int i = 0; i < points_window_copy.size() - 1; i++)
	{
		//check number of intersections
		count = circle_intersection(circle_center, circle_radius, points_window_copy[i], points_window_copy[i + 1], intersectPoints);

		found += count;

		if (count == 2)
		{
			points_solutions.push_back(intersectPoints[0]);
			points_solutions.push_back(intersectPoints[1]);
		}
			
		else if (count == 1)
		{
			points_solutions.push_back(intersectPoints[0]);
		}
	}

	/*if (found == 2)
	{
		Point firstPoint = Point(cos(0)*circle_radius, sin(0)*circle_radius);
		float length;

		glm::vec2 center_first_point = glm::vec2(firstPoint.x - circle_center.x, firstPoint.y - circle_center.y);
		length = sqrtf(center_first_point.x * center_first_point.x + center_first_point.y * center_first_point.y);
		center_first_point /= length;

		glm::vec2 center_inter_point1 = glm::vec2(points_solutions[0].x - circle_center.x, points_solutions[0].y - circle_center.y);
		length = sqrtf(center_inter_point1.x * center_inter_point1.x + center_inter_point1.y * center_inter_point1.y);
		center_inter_point1 /= length;

		glm::vec2 center_inter_point2 = glm::vec2(points_solutions[1].x - circle_center.x, points_solutions[1].y - circle_center.y);
		length = sqrtf(center_inter_point2.x * center_inter_point2.x + center_inter_point2.y * center_inter_point2.y);
		center_inter_point2 /= length;

		float angle1, angle2;

		angle1 = atan2f(points_solutions[0].y - circle_center.y, points_solutions[0].x - circle_center.x);
		angle2 = atan2f(points_solutions[1].y - circle_center.y, points_solutions[1].x - circle_center.x);
		angleDegr1 = (180 * angle1) / M_PI;
		angleDegr2 = (180 * angle2) / M_PI;

		if (angleDegr1 < 0)
			angleDegr1 += 360.0f;
		if (angleDegr2 < 0)
			angleDegr2 += 360.0f;
		std::cout << angleDegr1 << std::endl;
		std::cout << angleDegr2 << std::endl;

	}*/
}

int circle_intersection(const Point& circle_center, float circle_radius, const Point& line_start, const Point& line_end, std::vector<Point>& intersect_points)
{
	//Calcul du determinant
	const float dx = line_end.x - line_start.x;
	const float dy = line_end.y - line_start.y;

	const float startCenterX = line_start.x - circle_center.x;
	const float startCenterY = line_start.y - circle_center.y;

	const float A = dx * dx + dy * dy;
	const float B = 2.0f * (dx * startCenterX + dy * startCenterY);
	const float C = startCenterX * startCenterX + startCenterY * startCenterY - circle_radius * circle_radius;

	const float det = B * B - 4.0f * A * C;

	Point test_intersect_point;
	Point test_intersect_point2;

	if ((A <= 0.0000001f) || (det < 0.0f))
	{
		// No real solutions.
		return 0;
	}

	else if (det == 0.0f)
	{
		// One solution.
		const float t = -B / (2.0f * A);
		
		
		test_intersect_point.x = line_start.x + t * dx;
		test_intersect_point.y = line_start.y + t * dy;

		//Check if is_between because line of window extend to infinite
		if (is_between(line_start, test_intersect_point, line_end))
		{
			intersect_points.resize(1);
			intersect_points[0] = test_intersect_point;
			
			return 1;
		}
	}
	else
	{
		const float t1 = (float)((-B + sqrt(det)) / (2.0f * A));
		test_intersect_point.x = line_start.x + t1 * dx;
		test_intersect_point.y = line_start.y + t1 * dy;

		const float t2 = (-B - sqrt(det)) / (2.0f * A);
		test_intersect_point2.x = line_start.x + t2 * dx;
		test_intersect_point2.y = line_start.y + t2 * dy;

		//Check if is_between because line of window extend to infinite
		if (is_between(line_start, test_intersect_point, line_end) && is_between(line_start, test_intersect_point2, line_end))
		{
			intersect_points.resize(2);

			intersect_points[0] = test_intersect_point;
			intersect_points[1] = test_intersect_point2;
			return 2;
		}
		else if (is_between(line_start, test_intersect_point, line_end))
		{
			intersect_points.resize(1);
			intersect_points[0] = test_intersect_point;
			return 1;
		}
		else if (is_between(line_start, test_intersect_point2, line_end))
		{
			intersect_points.resize(1);
			intersect_points[0] = test_intersect_point2;
			return 1;
		}
	}
	return 0;
}

float square_distance(Point a, Point b)
{
	return sqrt(pow(a.x - b.x , 2) + pow(a.y - b.y, 2));
}
	
bool is_between(Point a, Point c, Point b)
{
	float result = (square_distance(a, c) + square_distance(c, b)) - square_distance(a, b);
	if (fabs((square_distance(a, c) + square_distance(c, b)) - square_distance(a, b)) <= 0.0001)
		return true;
	else
		return false;
}
