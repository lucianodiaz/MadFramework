#pragma once
#include <SFML/Graphics.hpp>
#include <string>


class World
{
public:
	World(int width, int height, const std::string& title);

	virtual ~World();

	void Run(int frame_per_seconds = 30);

private:

	void ProcessInput();

	void Update(float deltaTime);

	void Render();

	sf::RenderWindow window;


	bool m_isRunning;

};