#pragma once
#include <SFML/System/Vector2.hpp>



struct VelocityComponent : public IComponent
{
	sf::Vector2f velocity;
};