#pragma once
#include <string>
#include <memory>
#include "IScene.h"
#include <unordered_map>
#include "ISceneTransition.h"
#include <MadFrameworkExport.h>
class MAD_API SceneManager
{
public:

	SceneManager();

	void AddScene(const std::string& name, std::unique_ptr<IScene> scene);

	
	void ChangeScene(const std::string& sceneName);

    void ChangeSceneWithTransition(
        const std::string& sceneName,
        std::unique_ptr<ISceneTransition> outTransition,
        std::unique_ptr<ISceneTransition> inTransition
    );

	IScene* GetCurrentScene() const { return m_currentScene; }
	const std::string& GetCurrentSceneName() const { return m_currentSceneName; }
	bool HasScene(const std::string& name) const { return m_scenes.find(name) != m_scenes.end(); }

	void Update(float deltaTime);

	void Draw(sf::RenderWindow& window);

	void ProcessInput(sf::Event& event);

	const std::unique_ptr<ECSManager>& GetECSManager();
	TilemapManager& GetTilemapManager();

	void StartWithInternalSplash(std::unique_ptr<ISceneTransition> outT,
		std::unique_ptr<ISceneTransition> inT);

	// Check if there's a transition waiting for the current scene to be ready
	bool IsTransitionPending() const { return !m_pendingScene.empty(); }

	// Get the name of the scene we're trying to transition to
	const std::string& GetPendingSceneName() const { return m_pendingScene; }

	void DisableEngineSplashScreen();

	void CancelPendingTransition();
protected:
	friend class World;

	template<typename T>
	void AddUserWidget(std::shared_ptr<T> userWidget);

private:

	enum class TransitionPhase { Idle, Out, Switch, In };

	// Check current transition phase
	TransitionPhase GetTransitionPhase() const { return m_phase; }

	// Helpers
	void startOutPhaseIfNeeded();
	void switchScene();      
	void clearTransitions();   

	sf::View GetLetterBoxView(sf::View);

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
	std::string  m_pendingScene;

	bool m_isTransitionRunning = false;
	bool m_isTransitionOut = false;

	bool m_isSplashScreen = false;

	sf::Text fpsText;

	TransitionPhase m_phase{ TransitionPhase::Idle };
	std::unique_ptr<ISceneTransition> m_out;
	std::unique_ptr<ISceneTransition> m_in;

	sf::RectangleShape m_overlay;

private:
	void debugTransitionState();
	void switchToNewScene();
	void finishTransition();
	bool isTransitionFullyCovering(ISceneTransition* transition);
	void drawTransitions(sf::RenderWindow& window);
	void drawUI(sf::RenderWindow& window);
	void handleFirstUserSceneRequest(const std::string& sceneName,
		std::unique_ptr<ISceneTransition> outT,
		std::unique_ptr<ISceneTransition> inT);
	void startEngineWithInternalSplash();
	void createInternalSplashScene();
	const std::string kSplashName = "mad_splash_screen_01";
	bool m_bootWithSplash = false;      // estamos en modo “boot con splash”
	bool m_splashActive = false;      // splash es la escena actual
	// Pedido del usuario que se aplicará al terminar el splash:
	std::string m_afterSplashScene;
	std::unique_ptr<ISceneTransition> m_afterOut, m_afterIn;
	bool m_engineStartsWithSplash = true;
	bool m_firstUserSceneRequest = true;
	bool m_splashSceneCreated = false;
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
