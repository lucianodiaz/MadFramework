#pragma once
#include <SFML/System/Vector2.hpp>

struct TransformComponent : public IComponent
{
	sf::Vector2f position;
	float rotation;
};