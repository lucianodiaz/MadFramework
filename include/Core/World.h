#pragma once
#include <MadFrameworkExport.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <nlohmann/json.hpp>
#include <Input/ActionMap.h>
#include "TimerManager.h"
#include "SceneManager.h"
#include "TweenManager.h"
#include "SoundManager.h"
#include "ResourceManager.h"

class ECSManager;
class Actor;
class Action;
class Window;

class MAD_API World
{
public:

	virtual ~World();


	template<typename T>
	std::shared_ptr<T>& CreateGUI();

	template<typename T, typename... Args>
	T& SpawnActor(Args&&... args);

	const std::unique_ptr<ECSManager>& GetECSManager();

	Actor& GetActor(const Entity& entity);

	Actor& GetActorByTag(const std::string& tag);

	std::vector<Actor> GetActorsByTag(const std::string& tag);

	Window& GetWindow() const;


	static std::shared_ptr <World> GetWorld()
	{
		if (_world == nullptr)
		{
			_world = std::shared_ptr<World>(new World());
		}

		return _world;
	}

	inline void StartGame() { if (!m_wasRun)Run(m_fps); }

	sf::Texture& GetTexture(const std::string& id);
	sf::Music& GetMusic(const std::string& id);
	nlohmann::json& GetJson(const std::string& id);
	sf::Font& GetFont(const std::string& id);
	sf::SoundBuffer& GetSound(const std::string& id);
	sf::Shader& GetShader(const std::string& id);

	ActionMap<std::string>& GetActionsMap() { return m_actionsMap; }

	void LoadTexture(const std::string& name, const std::string& path);

	void LoadMusic(const std::string& name, const std::string& path);

	void LoadFont(const std::string& name, const std::string& path);

	void LoadSound(const std::string& name, const std::string& path);

	void LoadShader(const std::string& name, const std::string& fragPath);
	void LoadShader(const std::string& name, const std::string& vertPath, const std::string& fragPath);

	TimerManager& GetTimerManager() { return m_timerManager; }
	TweenManager& GetTweenManager() { return m_tweenManager; }

	void ShowFPS(bool show) { m_showFPS = show; }

	const bool IsShowFPS() const { return m_showFPS; }

	void QuitGame();

	void DisableEngineSplashScreen();

	TilemapManager& TilemapManager();

	const std::string& AssetsPath() { return m_defaultAssetsPath; }

	SceneManager& GetSceneManager() { return m_sceneManager; }

	SoundMananger& GetSoundManager() { return m_soundManager; }

	void SetFPS(unsigned int newFPS);

protected:
	World();

	void Run(int frame_per_seconds = 30);

	void CreateMainWindow(int width, int height, std::string name);

	void ProcessInput();

	void Update(float deltaTime);

	void Render();

	void LoadResources();

	void LoadInputs();


	template<typename T>
	std::shared_ptr<T> GetSystem();

	std::unique_ptr<Window> _window;

	bool m_isRunning;
	bool m_wasRun = false;

	bool m_showFPS = false;
	bool m_engineStartsWithSplash = true;

	std::vector<std::unique_ptr<Actor>> m_actors;
	std::vector<Actor*> m_actorToDestroy;

	static std::shared_ptr <World> _world;

	TimerManager m_timerManager;

	SceneManager m_sceneManager;

	TweenManager m_tweenManager;

	SoundMananger m_soundManager;

	ResourceManager<sf::Texture, std::string> m_textures;
	ResourceManager<sf::Music, std::string> m_musics;
	ResourceManager<sf::Font, std::string> m_fonts;
	ResourceManager<sf::SoundBuffer, std::string> m_sounds;
	ResourceManager<nlohmann::json, std::string> m_jsons;
	ResourceManager<sf::Shader, std::string> m_shaders;

	ActionMap<std::string> m_actionsMap;



	friend class Actor;
	friend class ISystem;
	friend class GameRunner;

private:
	const std::string m_internalAssetPath = "mad/internalAssets/";
	const std::string m_defaultAssetsPath = "assets/";
	void LoadInternalAssets();

	bool m_quit = false;

	unsigned int m_fps = 30;
};

#include "World.inl"