#include "Core/World.h"
#include "ECS/ECSManager.h"
#include <iostream>
#include <string>
#include <memory>
#include <ECS/Systems/MovementSystem.h>


World::World(int width, int height, const std::string& title) :
	window(sf::VideoMode(width,height),title),m_isRunning(true)
{


	ecs = std::make_unique<ECSManager>();

	if (!ecs)
	{
		std::cout << "Error creating ECS!.." << std::endl;
	}
	std::cout << "World is running!.." << std::endl;
}

World::~World()
{
	std::cout << "World is clossing" << std::endl;
}

void World::Run(int frame_per_seconds)
{
	sf::Clock clock;

	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	sf::Time TimerPerFrame = sf::seconds(1.f / frame_per_seconds);

	RegisterDefaultSystems();

	while (m_isRunning)
	{
		ProcessInput();
		bool repaint = false;
		timeSinceLastUpdate += clock.restart();

		while (timeSinceLastUpdate > TimerPerFrame)
		{
			timeSinceLastUpdate -= TimerPerFrame;
			repaint = true;
			Update(TimerPerFrame.asSeconds());
		}
		if (repaint)Render();
	}
}

void World::RegisterDefaultSystems()
{
	ecs->RegisterSystem<MovementSystem>(ecs);
}

void World::ProcessInput()
{
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			m_isRunning = false;
		}
	}
}

void World::Update(float deltaTime)
{
	ecs->Update(deltaTime);
}

void World::Render()
{
	window.clear();


	window.display();
}
