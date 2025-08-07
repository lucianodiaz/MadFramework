#pragma once
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <algorithm>

namespace MAD
{
	namespace MathUtils
	{
		inline float VectorLength(const sf::Vector2f& vector)
		{
			return std::sqrt(vector.x * vector.x + vector.y * vector.y);
		}

		inline float IsMoving(const sf::Vector2f& vector, float threshold = 0.01f)
		{
			return VectorLength(vector) >= threshold;
		}


		template<typename T>
		T& PickRandomFromVector(std::vector<T>& vector)
		{
			if (vector.empty())
			{
				std::cout << "Vector is empty, cannot pick a random element." << std::endl;
				T emptyValue;
				return emptyValue;
			}

			std::random_device rd;
			std::mt19937 generator(rd());

			std::uniform_int_distribution<> distribution(0, vector.size() - 1);

			int randomIndex = distribution(generator);

			T randomValue = vector[randomIndex];

			return randomValue;
		}
	}
}

