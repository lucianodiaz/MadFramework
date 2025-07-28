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
#include <ECS/Systems/CameraSystem.h>
#include <Core/Signal.h>
#include <Gameplay/Actor.h>
#include <Gameplay/SplashScreenFramework.h>
#include <Transitions/FadeTransition.h>
#include <Core/TilemapManager.h>

std::shared_ptr<World> World::_world = nullptr;

World::World() : m_isRunning(true)
{
	LoadInternalAssets();
	LoadResources();
	//CreateECSManager();
	/*RegisterDefaultSystems();*/
	LoadInputs();

	char path[MAX_PATH];
	GetModuleFileNameA(NULL, path, MAX_PATH);
	auto WindowName = std::filesystem::path(path).stem().string();

	CreateMainWindow(1920, 1080, WindowName);

	auto splashScene = std::make_unique<SplashScreenFramework>(2.5f);
	m_sceneManager.AddScene("mad_splash_screen_01", std::move(splashScene));
	

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
	sf::Clock fpsClock; 
	int frameCount = 0; 

	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	sf::Time TimerPerFrame = sf::seconds(1.f / frame_per_seconds);

	Signal::GetInstance().Dispatch("onFPSUpdate", frame_per_seconds);


	//I guess I will call Splash screen Here for one reason
	m_sceneManager.ChangeSceneWithTransition("mad_splash_screen_01", std::make_unique<FadeTransition>(Fade::In, 1.0f), std::make_unique<FadeTransition>(Fade::Out, 1.0f));


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
		if (repaint)
		{
			Render();
			frameCount++;
		}

		//Only for debug
		if (fpsClock.getElapsedTime().asSeconds() >= 1.f)
		{
			//std::cout << "FPS: " << frameCount << std::endl;
			Signal::GetInstance().Dispatch("onFPSUpdate", frameCount);
			frameCount = 0;
			fpsClock.restart();
			
		}
	}
}

void World::CreateMainWindow(int width, int height, std::string name)
{
	_window = std::make_unique<Window>(width, height, name);
}

//void World::CreateECSManager()
//{
//	/*ecs = std::make_unique<ECSManager>();
//
//	if (!ecs)
//	{
//		std::cout << "Error creating ECS!.." << std::endl;
//	}*/
//
//}

//void World::RegisterDefaultSystems()
//{
//	
//	/*m_sceneManager.GetECSManager()->RegisterSystem<MovementSystem>(ecs);
//	m_sceneManager.GetECSManager()->RegisterSystem<CollisionSystem>(ecs);
//	m_sceneManager.GetECSManager()->RegisterSystem<CameraSystem>(ecs);
//	m_sceneManager.GetECSManager()->RegisterSystem<RenderSystem>(ecs);
//	m_sceneManager.GetECSManager()->RegisterSystem<AnimationSystem>(ecs);*/
//
//}

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

	m_sceneManager.Update(deltaTime);

	m_sceneManager.GetECSManager()->Update(deltaTime); //update systems

	for (auto& actor : m_actors)
	{
		actor->Update(deltaTime); //update entities
	}
	m_tweenManager.Update(deltaTime); //update TweenManager
	m_timerManager.Update(); //update TimerManager
}

void World::Render()
{
	_window->Clear();

	

	//m_tilemapManager.Draw(_window->GetRenderWindow());

	//ecs->Draw(_window->GetRenderWindow());

	m_sceneManager.Draw(_window->GetRenderWindow());

	_window->Display();
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

TilemapManager& World::TilemapManager()
{
	// TODO: Insertar una instrucción "return" aquí
	return m_sceneManager.GetTilemapManager();
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
			auto key = MAD::InputUtils::GetKeyFromString(StringUtil::ToUpper(binding["value"]));

			m_actionsMap.map(actionName, key);
		}
		else if (StringUtil::ToLower(binding["type"]) == "mouse")
		{
			auto mouseButton = MAD::InputUtils::GetMouseButtonFromString(StringUtil::ToUpper(binding["value"]));
			m_actionsMap.map(actionName, mouseButton);
		}
		
	}

}

void World::LoadInternalAssets()
{

	m_fonts.Load("defaultFont", m_internalAssetPath + "default.ttf");
	m_textures.Load("splash", m_internalAssetPath + "splash.png");
}

const std::unique_ptr<ECSManager>& World::GetECSManager()
{
	// TODO: Insertar una instrucción "return" aquí

	return m_sceneManager.GetECSManager();

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

Actor& World::GetActorByTag(const std::string& tag)
{
	// TODO: Insertar una instrucción "return" aquí
	auto it = std::find_if(m_actors.begin(), m_actors.end(),
		[&tag](const std::unique_ptr<Actor>& actor) {
			return actor->GetGameTag() == tag;
		});

	return it != m_actors.end() ? **it : throw std::runtime_error("Actor with tag '" + tag + "' not found");
}

std::vector<Actor> World::GetActorsByTag(const std::string& tag)
{
	
	std::vector<Actor> actorsWithTag;

	for (auto& actor : m_actors)
	{
		if (actor->GetGameTag() == tag)
		{
			actorsWithTag.emplace_back(*actor);
		}
	}

	return actorsWithTag;
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
