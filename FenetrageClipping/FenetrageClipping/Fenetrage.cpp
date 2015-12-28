#include "Fenetrage.h"

float isVisble(Point p1, Point p2, Point p_to_test)
{

	/*float minX = points_window[0].x;
	float maxX = points_window[0].x;
	float minY = points_window[0].y;
	float maxY = points_window[0].y;
	for (size_t i = 1; i < points_window.size(); i++)
	{
	Point q = points_window[i];
	minX = std::min(q.x, minX);
	maxX = std::max(q.x, maxX);
	minY = std::min(q.y, minY);
	maxY = std::max(q.y, maxY);
	}

	//Check the extreme border
	if (point.x < minX || point.x > maxX || point.y < minY || point.y > maxY)
	{
	return false;
	}

	bool inside = false;
	for (size_t i = 0, j = points_window.size() - 1; i < points_window.size(); j = i++)
	{
	if ((points_window[i].y > point.y) != (points_window[j].y > point.y) && point.x < (points_window[j].x - points_window[i].x) * (point.y - points_window[i].y) / (points_window[j].y - points_window[i].y) + points_window[i].x)
	{
	inside = !inside;
	}
	}*/
	glm::vec2 AB = { p2.x - p1.x, p2.y - p1.y };


	int length = sqrtf(AB.x * AB.x + AB.y * AB.y);
	AB /= length;

	glm::vec2 normal_ext(AB.y, -AB.x);
	glm::vec2 normal_int(-AB.y, AB.x);

	/*	float dotProd = glm::dot(AB, normal_int);
	std::cout << dotProd << std::endl;*/

	glm::vec2 droite(p1.x - p_to_test.x, p1.y - p_to_test.y);

	float result = glm::dot(normal_int, droite);
	return result;
}

bool cut(Point p1, Point p2, Point p3, Point p4, glm::vec2 &result)
{
	glm::mat2 intersect = glm::mat2(p2.x - p1.x, -(p4.x - p3.x), p2.y - p1.y, -(p4.y - p3.y));
	glm::mat2 intersectInverse = glm::inverse(intersect);
	float determinant = glm::determinant(intersect);
	if (determinant == 0)
		std::cout << "Matrice non inversible" << std::endl;

	glm::vec2 vec = { p3.x - p1.x, p3.y - p1.y };
	result = vec*intersectInverse;

	float t = result.x;
	float s = result.y;

	bool is_cut = false;

	if (t > 0 && t < 1 && s > 0 && s < 1)
		is_cut = true;
	else
		is_cut = false;
	return is_cut;
}

std::vector< Point >  Fenetrage(std::vector< Point > points_window, std::vector< Point > points_poly, bool &finish_fenetrage)
{
	std::vector< Point > points_solution;
	int nb_points_window = points_window.size();
	int nb_points_poly = points_poly.size();
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
				save_point = points_poly[i];
			else
			{
				Point p1 = points_poly[i - 1];
				Point p2 = points_poly[i];

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
					//std::cout << "Point poly " << i - 1 << " secant avec Point window " << j << std::endl;
				}
				/*else
				{
					std::cout << "Point poly " << i - 1 << " pas secant avec Point window " << j << std::endl;
				}*/
			}

			std::cout << "Point window " << j << std::endl;
			Point p3 = points_window[j];
			Point p4;
			if (j + 1 == nb_points_window)
				p4 = save_point_window;
			else
				p4 = points_window[j + 1];

			if (isVisble(p3, p4, points_poly[i]) > 0)
			{
				points_solution.push_back(points_poly[i]);
				nb_points_solutions++;
				std::cout << "Point poly " << i << " visible" << std::endl;
			}

			else
				std::cout << "Point poly " << i << " pas visible" << std::endl;

		}
		if (nb_points_solutions > 0)
		{
			Point p1 = save_point;
			Point p2 = points_poly[nb_points_poly - 1];

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
				//std::cout << "Point poly " << nb_points_poly - 1 << " secant avec Point window " << j << std::endl;
			}
			/*else
			{
				std::cout << "Point poly " << nb_points_poly - 1 << " pas secant avec Point window " << j << std::endl;
			}*/

			points_poly = points_solution;
			nb_points_poly = nb_points_solutions;

		}
	}

	finish_fenetrage = true;
	return points_solution;
}
