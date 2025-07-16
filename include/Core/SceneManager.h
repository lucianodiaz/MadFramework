#pragma once
#include <string>
#include <memory>
#include "IScene.h"
#include <unordered_map>
#include "ISceneTransition.h"




class SceneManager
{
public:

	SceneManager() {};

	void AddScene(const std::string& name, std::unique_ptr<IScene> scene);

	void ChangeScene(const std::string& sceneName);

	void ChangeSceneWithTransition(const std::string& nextScene, std::unique_ptr<ISceneTransition> transitionIn, std::unique_ptr<ISceneTransition> transitionOut);

	void Update(float deltaTime);

	void Draw(sf::RenderWindow& window);

	const std::unique_ptr<ECSManager>& GetECSManager();
	TilemapManager& GetTilemapManager();
private:
	std::unordered_map<std::string, std::unique_ptr<IScene>> m_scenes;

	IScene* m_currentScene = nullptr;

	std::unique_ptr<ISceneTransition> m_transitionIn = nullptr;
	std::unique_ptr<ISceneTransition> m_transitionOut = nullptr;

	std::unique_ptr<ISceneTransition> m_currentTransition = nullptr;

	std::unique_ptr < ISceneTransition> m_splashTransitionIn = nullptr;
	std::unique_ptr < ISceneTransition> m_splashTransitionOut = nullptr;

	std::string m_currentSceneName;
	std::string m_nextSceneName;
	std::string m_splashSceneName;

	bool m_isTransitionRunning = false;
	bool m_isTransitionOut = false;

	bool m_isSplashScreen = false;
};