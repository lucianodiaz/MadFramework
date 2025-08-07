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

		//You can use this to get a random value in a Indexed Container std::vector,std::array,std::deque
		template<typename Container>
		auto& PickRandom(Container& container)
		{
			if (container.empty())
			{
				throw std::runtime_error("Container is empty, cannot pick a random element.");
			}

			static std::random_device rd;
			static std::mt19937 generator(rd());

			std::uniform_int_distribution<> distribution(0, static_cast<int>(container.size()) - 1);

			return container[distribution(generator)];
		}

		//You can use this to get a random value in a iterator Container std::list,std::unordered_map
		template<typename Container>
		auto& PickRandomIterable(Container& container)
		{
			if (container.empty())
			{
				throw std::runtime_error("Container is empty, cannot pick a random element.");
			}

			static std::random_device rd;
			static std::mt19937 generator(rd());

			std::uniform_int_distribution<> distribution(0, static_cast<int>(container.size()) - 1);

			auto it = container.begin();
			std::advance(it, distribution(generator));
			return *it;
		}
	}
}

