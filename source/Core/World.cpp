#include "Core/World.h"
#include "ECS/ECSManager.h"
#include <iostream>
#include <string>
#include <memory>
#include <ECS/Systems/MovementSystem.h>
#include <ECS/Systems/RenderSystem.h>


std::shared_ptr<World> World::_world = nullptr;

World::World() : m_isRunning(true)
{
	CreateECSManager();
	RegisterDefaultSystems();
	LoadResources();
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
	ecs->RegisterSystem<RenderSystem>(ecs);
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

	ecs->Draw(_window->GetRenderWindow());

	_window->Display();
}

void World::Draw()
{
	auto renderSystem = GetSystem<RenderSystem>();
	if (renderSystem)
	{
		renderSystem->Render(_window->GetRenderWindow());
	}

}

void World::LoadResources()
{
	std::cout << "Loading resources..." << std::endl;

	try
	{
		m_textures.LoadFromJson("resources.json");
		m_musics.LoadFromJson("resources.json");
		//m_fonts.LoadFromJson("resources.json");
		//m_sounds.LoadFromJson("resources.json");
		//m_jsons.LoadFromJson("resources.json");
	}
	catch (const std::exception& e)
	{
		std::cerr<<"error loading resources: " << e.what() << std::endl;
	}
}

sf::Texture& World::GetTexture(const std::string& id)
{
	return m_textures.Get(id);
}

sf::Music& World::GetMusic(const std::string& id)
{
	return m_musics.Get(id);
}

nlohmann::json& World::GetJson(const std::string& id)
{
	return m_jsons.Get(id);
}

sf::Font& World::GetFont(const std::string& id)
{
	return m_fonts.Get(id);
}

sf::SoundBuffer& World::GetSound(const std::string& id)
{
	return m_sounds.Get(id);
}
