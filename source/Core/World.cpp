#include "Core/World.h"
#include "ECS/ECSManager.h"
#include <iostream>
#include <string>
#include <memory>
#include <ECS/Systems/MovementSystem.h>
#include <ECS/Systems/RenderSystem.h>
#include <windows.h>
#include <ECS/Systems/CollisionSystem.h>
#include <inputPrivateUtil.h>
#include <ECS/Systems/AnimationSystem.h>

std::shared_ptr<World> World::_world = nullptr;

World::World() : m_isRunning(true)
{
	LoadResources();
	CreateECSManager();
	RegisterDefaultSystems();
	LoadInputs();
	char path[MAX_PATH];
	GetModuleFileNameA(NULL, path, MAX_PATH);
	auto WindowName = std::filesystem::path(path).stem().string();

	CreateMainWindow(1920, 1080, WindowName);

	std::cout << "App is running!.." << std::endl;
}

World::~World()
{
	std::cout << "App is clossing" << std::endl;
}


void World::Run(int frame_per_seconds)
{
	m_wasRun = true;

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

}

void World::RegisterDefaultSystems()
{
	ecs->RegisterSystem<MovementSystem>(ecs);
	ecs->RegisterSystem<RenderSystem>(ecs);
	ecs->RegisterSystem<CollisionSystem>(ecs);
	ecs->RegisterSystem<AnimationSystem>(ecs);
}

void World::ProcessInput()
{
	sf::Event event;

	while (_window->pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			_window->Close();
		}
	}

	for (auto& actor : m_actors)
	{
		actor->ProcessInput();
	}

}

void World::Update(float deltaTime)
{
	ecs->Update(deltaTime);
	for (auto& actor : m_actors)
	{
		actor->Update(deltaTime);
	}
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
		m_fonts.LoadFromJson("resources.json");
		m_sounds.LoadFromJson("resources.json");
		m_jsons.LoadFromJson("resources.json");
	}
	catch (const std::exception& e)
	{
		std::cerr<<"error loading resources: " << e.what() << std::endl;
	}
}

void World::LoadTexture(const std::string& name, const std::string& path)
{
	try
	{
		m_textures.Load(name, path);
	}
	catch (const std::exception& e)
	{
		std::cerr << "error loading resource: " << e.what() << std::endl;
	}

}

void World::LoadMusic(const std::string& name, const std::string& path)
{
	try
	{
		m_musics.Load(name, path);
	}
	catch (const std::exception& e)
	{
		std::cerr << "error loading resource: " << e.what() << std::endl;
	}
}

void World::LoadFont(const std::string& name, const std::string& path)
{
	try
	{
		m_fonts.Load(name, path);
	}
	catch (const std::exception& e)
	{
		std::cerr << "error loading resource: " << e.what() << std::endl;
	}
}

void World::LoadSound(const std::string& name, const std::string& path)
{
	try
	{
		m_sounds.Load(name, path);
	}
	catch (const std::exception& e)
	{
		std::cerr << "error loading resource: " << e.what() << std::endl;
	}
}

void World::LoadInputs()
{
	std::ifstream file("inputs.json");
	if (!file.is_open())
	{
		std::cerr << "Error opening inputs.json" << std::endl;
		return;
	}

	nlohmann::json jsonData;

	file >> jsonData;

	for (auto& [actionName, binding] : jsonData.items())
	{
		if (StringUtil::ToLower(binding["type"]) == "keyboard")
		{
			auto key = GetKeyFromString(StringUtil::ToUpper(binding["value"]));

			m_actionsMap.map(actionName, key);
		}
		else if (StringUtil::ToLower(binding["type"]) == "mouse")
		{
			auto mouseButton = GetMouseButtonFromString(StringUtil::ToUpper(binding["value"]));
			m_actionsMap.map(actionName, mouseButton);
		}
		
	}

}

Actor& World::GetActor(const Entity& entity)
{
	auto it = std::find_if(m_actors.begin(), m_actors.end(),
		[&entity](const std::unique_ptr<Actor>& actor) {
			return actor->GetEntity() == entity;
		});

	if (it != m_actors.end())
	{
		return **it;
	}
	else
	{
		throw std::runtime_error("Actor not found");
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
