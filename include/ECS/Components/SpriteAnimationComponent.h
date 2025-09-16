#pragma once
#include <ECS/Component.h>
#include <SFML/Graphics.hpp>
#include <Core/World.h>

struct AnimationData
{
	sf::IntRect frameRect; // Rectangle defining the current frame in the texture
	float elapsedTime = 0.0f; // Time elapsed since the last frame change
	float frameDuration = 0.1f; // Duration of each frame in seconds
	bool isLooping = true; // Whether the animation should loop
	bool isPlaying = false; // Whether the animation is currently playing
	bool isPaused = false; // Whether the animation is paused
	int currentFrame = 0; // Index of the current frame in the animation
	int totalFrames = 0; // Total number of frames in the animation

	int spriteSizeX = 0;
	int spriteSizeY = 0; // Size of the sprite in pixels

	std::string name; // Name of the animation for identification

	sf::Texture texture; // Texture for the animation only if you have all animations in png/files separated
	sf::Sprite sprite; // Sprite to display the animation only if you have all animations in png/files separated
};

struct SpriteAnimationComponent : public IComponent
{
	SpriteAnimationComponent() = default; // Default constructor for the component

	SpriteAnimationComponent(sf::Texture& _texture, sf::IntRect _frameRect) : 
		texture(_texture), frameRect(_frameRect)
	{
		sprite.setTexture(texture);
		sprite.setTextureRect(frameRect);
	}

	void SetShader(sf::Shader& shader)
	{
		this->shader = &shader;
	}

	void SetShader(const std::string& shaderName)
	{
		shader = &World::GetWorld()->GetShader(shaderName);
	}

	sf::Shader* shader = nullptr; // Optional shader for the sprite animation
	std::string currentAnimation; // Name of the current animation being played
	std::unordered_map<std::string,AnimationData> animations; // List of animations for the sprite
	sf::IntRect frameRect; // Rectangle defining the current frame in the texture
	sf::Texture texture; // Texture for the sprite animation
	sf::Sprite sprite; // Sprite to display the animation
	bool visible = true;
};