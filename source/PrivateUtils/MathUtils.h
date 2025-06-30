#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>


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
	}
}

