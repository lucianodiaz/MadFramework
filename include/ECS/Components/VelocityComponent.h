#pragma once
#include <SFML/System/Vector2.hpp>



struct VelocityComponent : public IComponent
{
	VelocityComponent(float x, float y) {

		velocity.x = x;
		velocity.y = y;
	}

	sf::Vector2f velocity{};
};