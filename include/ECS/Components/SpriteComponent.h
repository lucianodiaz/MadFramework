#pragma once

#include "ECS/Component.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <Core/World.h>



struct SpriteComponent : public IComponent
{
	SpriteComponent(sf::Texture& _texture)
	{
		texture = _texture;
		sprite.setTexture(texture);
	}

	void SetShader(sf::Shader& shader)
	{
		this->shader = &shader;
	}

	void SetShader(const std::string& shaderName)
	{
		shader = &World::GetWorld()->GetShader(shaderName);
	}

	sf::Shader* shader = nullptr;
	sf::Texture texture;
	sf::Sprite sprite;

	bool visible = true;
};