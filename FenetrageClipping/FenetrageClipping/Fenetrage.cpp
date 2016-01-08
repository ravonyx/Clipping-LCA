#include "Fenetrage.h"

float isVisble(Point p1, Point p2, Point p_to_test)
{
	glm::vec2 AB = { p2.x - p1.x, p2.y - p1.y };


	int length = sqrtf(AB.x * AB.x + AB.y * AB.y);
	AB /= length;

	glm::vec2 normal_ext(AB.y, -AB.x);
	glm::vec2 normal_int(-AB.y, AB.x);

	glm::vec2 droite(p1.x - p_to_test.x, p1.y - p_to_test.y);

	float result = glm::dot(normal_int, droite);
	return result;
}

bool isInside(Point point, std::vector< Point > &points)
{
	bool inside = false;
	int i, j;
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

	if (t > 0 && t < 1 && s > 0 && s < 1)
		is_cut = true;
	else
		is_cut = false;
	return is_cut;
}

void Fenetrage(std::vector< Point > &points_solution, std::vector< Point > &points_window, std::vector< Point > points_poly, bool &finish_fenetrage)
{
	int nb_points_window = points_window.size();
	int nb_points_poly = points_poly.size();
	std::vector<Point> points_poly_copy = points_poly;
	int nb_points_solutions;

	std::cout << "Nb points poly " << nb_points_poly << std::endl;
	nb_points_solutions = 0;

	Point save_point;
	Point save_point_window;
	for (size_t j = 0; j < nb_points_window; j++)
	{
		points_solution.clear();
		nb_points_solutions = 0;

		if (j == 0)
			save_point_window = points_window[j];
		for (size_t i = 0; i < nb_points_poly; i++)
		{
			

			if (i == 0)
				save_point = points_poly_copy[i];
			else
			{
				Point p1 = points_poly_copy[i - 1];
				Point p2 = points_poly_copy[i];

				Point p3 = points_window[j];
				Point p4;
				if (j + 1 == nb_points_window)
					p4 = save_point_window;
				else
					p4 = points_window[j + 1];
				glm::vec2 res1;
				if (cut(p1, p2, p3, p4, res1))
				{
					float t = res1.x;
					float s = res1.y;

					glm::vec2 AB = { p2.x - p1.x, p2.y - p1.y };
					glm::vec2 part = AB * t;
					glm::vec2 intersect = { p1.x + part.x, p1.y + part.y };
					points_solution.push_back(Point(intersect.x, intersect.y));
					nb_points_solutions++;
				}
			}

			std::cout << "Point window " << j << std::endl;
			Point p3 = points_window[j];
			Point p4;
			if (j + 1 == nb_points_window)
				p4 = save_point_window;
			else
				p4 = points_window[j + 1];

			if (isVisble(p3, p4, points_poly_copy[i]) > 0)
			{
				points_solution.push_back(points_poly_copy[i]);
				nb_points_solutions++;
				std::cout << "Point poly " << i << " visible" << std::endl;
			}

			else
				std::cout << "Point poly " << i << " pas visible" << std::endl;

		}
		if (nb_points_solutions > 0)
		{
			Point p1 = save_point;
			Point p2 = points_poly_copy[nb_points_poly - 1];

			Point p3 = points_window[j];
			Point p4;
			if (j + 1 == nb_points_window)
				p4 = save_point_window;
			else
				p4 = points_window[j + 1];

			glm::vec2 res2;
			if (cut(p1, p2, p3, p4, res2))
			{
				float t = res2.x;
				float s = res2.y;

				glm::vec2 AB = { p2.x - p1.x, p2.y - p1.y };
				glm::vec2 part = AB * t;
				glm::vec2 intersect = { p1.x + part.x, p1.y + part.y };
				points_solution.push_back(Point(intersect.x, intersect.y));
				nb_points_solutions++;
			}
			points_poly_copy = points_solution;
			nb_points_poly = nb_points_solutions;

		}
	}

	Point inside;
	bool found = false;
	for (size_t i = 0; i < points_window.size(); i++)
	{
		if (isInside(points_window[i], points_poly))
		{
			inside = points_window[i];
			found = true;
		}
		
	}
	std::cout << " Found " << found << std::endl;
	if (found)
	{
		int lastDiff = 5000, diff = 0, index_to_add = -1;
		Point point_to_add;
		for (size_t i = 0; i < points_solution.size(); i++)
		{
			diff = sqrt(pow(points_solution[i].x - inside.x, 2) + pow(points_solution[i].y - inside.y, 2));
			std::cout << diff << std::endl;
			if (diff < lastDiff)
			{
				index_to_add = i;
				lastDiff = diff;
			}	
		}
		points_solution[index_to_add] = inside;
	}
	
	finish_fenetrage = true;
}
