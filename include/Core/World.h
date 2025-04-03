#pragma once
#include <SFML/Graphics.hpp>
#include <Gameplay/Actor.h>
#include <Window/Window.h>
#include <SFML/Audio.hpp>
#include <nlohmann/json.hpp>
#include "ResourceManager.h"


class ECSManager;

class World
{
public:

	virtual ~World();

	template<typename T, typename... Args>
	T& SpawnActor(Args&&... args);

	const std::unique_ptr<ECSManager>& GetECSManager() { return ecs; };

	static std::shared_ptr <World> GetWorld() 
	{
		if (_world == nullptr)
		{
			_world = std::shared_ptr<World>(new World());
		}

		return _world;
	}

	inline void StartGame() { if(!m_wasRun)Run(); }

	sf::Texture& GetTexture(const std::string& id);
	sf::Music& GetMusic(const std::string& id);
	nlohmann::json& GetJson(const std::string& id);
	sf::Font& GetFont(const std::string& id);
	sf::SoundBuffer& GetSound(const std::string& id);

protected:
	World();

	void Run(int frame_per_seconds = 30);

	void CreateMainWindow(int width,int height, std::string name);

	void CreateECSManager();

	void RegisterDefaultSystems();

	void ProcessInput();

	void Update(float deltaTime);

	void Render();

	void Draw();

	void LoadResources();

	template<typename T>
	std::shared_ptr<T> GetSystem();

	std::unique_ptr<Window> _window;

	std::unique_ptr<ECSManager> ecs;

	bool m_isRunning;
	bool m_wasRun = false;

	std::vector<std::unique_ptr<Actor>> m_actors;
	static std::shared_ptr <World> _world;


	ResourceManager<sf::Texture, std::string> m_textures;
	ResourceManager<sf::Music, std::string> m_musics;
	ResourceManager<sf::Font, std::string> m_fonts;
	ResourceManager<sf::SoundBuffer, std::string> m_sounds;
	ResourceManager<nlohmann::json, std::string> m_jsons;


	friend class Actor;
	friend class ISystem;
	friend class GameRunner;
};


template<typename T, typename ...Args>
inline T& World::SpawnActor(Args && ...args)
{
	static_assert(std::is_base_of<Actor, T>::value, "T must derive from Actor");

	m_actors.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));

	return static_cast<T&>(*m_actors.back());
}

template<typename T>
inline std::shared_ptr<T> World::GetSystem()
{
	for (auto& system : ecs->GetSystems())
	{
		if (auto castedSystem = std::dynamic_pointer_cast<T>(system))
		{
			return castedSystem;
		}
	}
	
	return nullptr;
}
