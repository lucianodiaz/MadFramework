#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <cstdint>


struct ParticleInstance
{
	sf::Vector2f pos{};
	sf::Vector2f vel{};

	float rot = 0.0f;
	float rotSpeed = 0.0f;
	float size = 1.0f;
	float age = 0.0f;
	float lifetime = 1.0f;
	sf::Color color = sf::Color::White;
	bool active = false;
};


struct EmitterSettings
{
	//Spawn
	float spawnRate = 100.0f;
	std::uint32_t maxParticles = 500;
	bool burst = false;
	std::uint32_t burstCount = 50;

	//ranges
	sf::Vector2f velMin{ -50.0f,-50.f }, velMax{ 50.0f,50.0f };
	float lifetimeMin = 0.5f, lifetimeMax = 1.5f;
	float sizeMin = 2.0f, sizeMax = 6.0f;
	float rotMin = 0.0f, rotMax = 0.0f;
	float rotSpeedMin = 0.0f, rotSpeedMax = 0.0f;
	sf::Color color = sf::Color::White;

	//forces
	sf::Vector2f gravity{ 0.0f,0.0f };
	float drag = 0.0f;

	bool asQuads = true;
};