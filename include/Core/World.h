#pragma once
#include "ResourceManager.h"
#include "TimerManager.h"
#include "SceneManager.h"
#include "TweenManager.h"
#include <MadFrameworkExport.h>
#include <Input/Action.h>
#include <Input/ActionMap.h>
#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Window/Window.h>

class ECSManager;
class Actor;


class MAD_API World
{
public:

	virtual ~World();

	template<typename T, typename... Args>
	T& SpawnActor(Args&&... args);

	const std::unique_ptr<ECSManager>& GetECSManager();

	Actor& GetActor(const Entity& entity);

	Actor& GetActorByTag(const std::string& tag);

	std::vector<Actor> GetActorsByTag(const std::string& tag);

	Window& GetWindow() const { return *_window; }


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

	ActionMap<std::string>& GetActionsMap() { return m_actionsMap; }

	void LoadTexture(const std::string& name, const std::string& path);

	void LoadMusic(const std::string& name, const std::string& path);

	void LoadFont(const std::string& name, const std::string& path);

	void LoadSound(const std::string& name, const std::string& path);

	TimerManager& GetTimerManager() { return m_timerManager; }
	TweenManager& GetTweenManager() { return m_tweenManager; }

	void ShowFPS(bool show) { m_showFPS = show; }

	const bool IsShowFPS() const { return m_showFPS; }

	TilemapManager& TilemapManager();

	const std::string& AssetsPath() { return m_defaultAssetsPath; }

	SceneManager& GetSceneManager() { return m_sceneManager; }

	
protected:
	World();

	void Run(int frame_per_seconds = 30);

	void CreateMainWindow(int width,int height, std::string name);

	//void CreateECSManager();

	//void RegisterDefaultSystems();

	void ProcessInput();

	void Update(float deltaTime);

	void Render();

	void LoadResources();

	void LoadInputs();


	template<typename T>
	std::shared_ptr<T> GetSystem();

	std::unique_ptr<Window> _window;

	//std::unique_ptr<ECSManager> ecs;

	bool m_isRunning;
	bool m_wasRun = false;

	bool m_showFPS = false;

	std::vector<std::unique_ptr<Actor>> m_actors;
	static std::shared_ptr <World> _world;

	TimerManager m_timerManager;

	SceneManager m_sceneManager;

	TweenManager m_tweenManager;

	ResourceManager<sf::Texture, std::string> m_textures;
	ResourceManager<sf::Music, std::string> m_musics;
	ResourceManager<sf::Font, std::string> m_fonts;
	ResourceManager<sf::SoundBuffer, std::string> m_sounds;
	ResourceManager<nlohmann::json, std::string> m_jsons;

	ActionMap<std::string> m_actionsMap;



	friend class Actor;
	friend class ISystem;
	friend class GameRunner;

	private:
	const std::string m_internalAssetPath = "mad/internalAssets/";
	const std::string m_defaultAssetsPath = "assets/";
	void LoadInternalAssets();
};


template<typename T, typename ...Args>
inline T& World::SpawnActor(Args && ...args)
{
	static_assert(std::is_base_of<Actor, T>::value, "T must derive from Actor");

	m_actors.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
	m_actors.back()->Start();
	return static_cast<T&>(*m_actors.back());
}

template<typename T>
inline std::shared_ptr<T> World::GetSystem()
{
	for (auto& system : m_sceneManager.GetECSManager()->GetSystems())
	{
		if (auto castedSystem = std::dynamic_pointer_cast<T>(system))
		{
			return castedSystem;
		}
	}
	
	return nullptr;
}
