#pragma once
#include <SFML/System/Vector2.hpp>



struct VelocityComponent : public IComponent
{
	VelocityComponent(float x, float y,float spd = 100.0f) {

		speed = spd;
		velocity.x = x;
		velocity.y = y;
	}

	sf::Vector2f velocity{};
	float speed = 100.f;
};