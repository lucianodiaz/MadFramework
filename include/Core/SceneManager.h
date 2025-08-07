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

	void ProcessInput(sf::Event& event);

	const std::unique_ptr<ECSManager>& GetECSManager();
	TilemapManager& GetTilemapManager();
protected:
	friend class World;

	template<typename T>
	void AddUserWidget(std::shared_ptr<T> userWidget);

private:

	sf::View& GetLetterBoxView(sf::View);

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

template<typename T>
inline void SceneManager::AddUserWidget(std::shared_ptr<T> userWidget)
{
	static_assert(std::is_base_of<UserWidget, T>::value, "T must derive from UserWidget");
	if (m_currentScene)
	{
		m_currentScene->m_userWidgets.emplace_back(userWidget);
		m_currentScene->m_userWidgets.back()->OnConstruct();
	}
}
