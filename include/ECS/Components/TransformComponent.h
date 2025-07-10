#pragma once
#include <SFML/System/Vector2.hpp>

struct TransformComponent : public IComponent
{

	TransformComponent()
	{
		position.x = 0;
		position.y = 0;
		rotation = 0;
	}

	TransformComponent(sf::Vector2f pos) : position(pos), rotation(0) 
	{
	}

	TransformComponent(float x, float y):rotation(0) {
		position.x = x;
		position.y = y;
	}

	TransformComponent(float x, float y, float rot)
	{
		position.x = x;
		position.y = y;
		rotation = rot;
	}


	void setPosition(const sf::Vector2f& pos) { position = pos; };
	void setPosition(float x, float y) { position.x = x; position.y = y; };

	sf::Vector2f position;
	float rotation{};
	bool isDirty{ true }; // Flag to indicate if the transform has changed since the last update
};