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
		inline void rand_init() {
			srand(time(NULL));
		};

		inline float random(float min, float max) { return (rand() / float(RAND_MAX)) * (max - min) + min; }
		inline int random(int min, int max) { return rand() % (max - min + 1) + min; }

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
				throw std::runtime_error("Vector is empty, cannot pick a random element.");
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

