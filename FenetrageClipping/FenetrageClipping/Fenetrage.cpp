#include "Fenetrage.h"

float isVisble(Point p1, Point p2, Point p_to_test)
{
	glm::vec2 AB = { p2.x - p1.x, p2.y - p1.y };


	float length = sqrtf(AB.x * AB.x + AB.y * AB.y);
	AB /= length;

	//glm::vec2 normal_ext(AB.y, -AB.x);
	glm::vec2 normal_int(-AB.y, AB.x);

	glm::vec2 droite(p1.x - p_to_test.x, p1.y - p_to_test.y);

	float result = glm::dot(normal_int, droite);
	return result;
}

bool isInside(Point point, std::vector< Point > &points)
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
	if ((sIn && tIn) || (!sIn && tIn))
		is_cut = true;
	else
		is_cut = false;
	return is_cut;
}

void add_intersection(Point p1, Point p2, std::vector< Point > &points_solution, float t)
{
	glm::vec2 AB = { p2.x - p1.x, p2.y - p1.y };
	glm::vec2 part = AB * t;
	glm::vec2 intersect = { p1.x + part.x, p1.y + part.y };
	points_solution.push_back(Point(intersect.x, intersect.y));
}
void Fenetrage(std::vector< Point > &points_solution, std::vector< Point > &points_window, std::vector< Point > &points_poly, bool &finish_fenetrage)
{
	size_t nb_points_poly = points_poly.size();

	std::vector<Point> points_win_copy = points_window;
	points_win_copy.push_back(points_window[0]);
	size_t nb_points_window = points_win_copy.size();

	std::vector<Point> points_poly_copy = points_poly;
	int nb_points_solutions;

	std::cout << "Nb points poly " << nb_points_poly << std::endl;
	std::cout << "Nb points window " << nb_points_window << std::endl;
	nb_points_solutions = 0;

	Point prev_point_poly;
	Point f;
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
			if (i == 0)
				f = current_point_poly;
			else
			{
				glm::vec2 res1;
				//std::cout << "Test " << "Point win " << j << "," << j + 1 << " with " << i - 1 << "," << i << std::endl;
				if (cut(prev_point_poly, current_point_poly, p3, p4, res1))
				{
					add_intersection(prev_point_poly, current_point_poly, points_solution, res1.x);
					std::cout << "Point win " << j << "," << j + 1 << " cut with Point poly " << i - 1 << "," << i << std::endl;
					nb_points_solutions++;
				}
			}
			last_index_poly = i;
			prev_point_poly = current_point_poly;
			if (isVisble(p3, p4, prev_point_poly) > 0)
			{
				points_solution.push_back(prev_point_poly);
				nb_points_solutions++;
			}
		}
		if (nb_points_solutions > 0)
		{
			glm::vec2 res2;
			if (cut(prev_point_poly, f, p3, p4, res2))
			{
				add_intersection(prev_point_poly, f, points_solution, res2.x);
				std::cout << "Points soluce > 0 Point win " << j << "," << j + 1 << " cut with Point poly " << last_index_poly << "," << 0 << std::endl;
				nb_points_solutions++;
			}
		}
		points_poly_copy = points_solution;
		nb_points_poly = nb_points_solutions;
	}
	
	finish_fenetrage = true;
}

