#include "Core/World.h"
#include "ECS/ECSManager.h"
#include <iostream>
#include <string>
#include <memory>
#include <ECS/Systems/MovementSystem.h>


std::shared_ptr<World> World::_world = nullptr;

World::World() : m_isRunning(true)
{
	CreateECSManager();
	RegisterDefaultSystems();

	CreateMainWindow(1920, 1080, "test");
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


	while (_window->IsOpen())
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

void World::CreateMainWindow(int width, int height, std::string name)
{
	_window = std::make_unique<Window>(width, height, name);
}

void World::CreateECSManager()
{
	ecs = std::make_unique<ECSManager>();

	if (!ecs)
	{
		std::cout << "Error creating ECS!.." << std::endl;
	}
	std::cout << "World is running!.." << std::endl;
}

void World::RegisterDefaultSystems()
{
	ecs->RegisterSystem<MovementSystem>(ecs);
}

void World::ProcessInput()
{
	sf::Event event;

	while (_window->pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			_window->Close();
		}
	}

}

void World::Update(float deltaTime)
{
	ecs->Update(deltaTime);
}

void World::Render()
{
	_window->Clear();


	_window->Display();
}
