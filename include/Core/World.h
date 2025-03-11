#pragma once
#include <SFML/Graphics.hpp>


class ECSManager;


class World
{
public:
	World(int width, int height, const std::string& title);

	virtual ~World();

	void Run(int frame_per_seconds = 30);

private:

	void RegisterDefaultSystems();

	void ProcessInput();

	void Update(float deltaTime);

	void Render();

	sf::RenderWindow window;

	std::unique_ptr<ECSManager> ecs;

	bool m_isRunning;

};