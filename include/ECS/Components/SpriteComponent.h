#include "ECS/Component.h"
#include <SFML/Graphics.hpp>
#include <string>



struct SpriteComponent : public IComponent
{
	SpriteComponent(sf::Texture& _texture)
	{
		texture = _texture;
		sprite.setTexture(texture);
	}

	sf::Texture texture;
	sf::Sprite sprite;
};