#include <Core/SceneManager.h>
#include <iostream>
#include <ECS/Systems/MovementSystem.h>
#include <ECS/Systems/CollisionSystem.h>
#include <ECS/Systems/CameraSystem.h>
#include <ECS/Systems/RenderSystem.h>
#include <ECS/Systems/AnimationSystem.h>


const std::unique_ptr<ECSManager>& SceneManager::GetECSManager()
{
	// TODO: Insertar una instrucción "return" aquí
	return m_scenes[m_currentSceneName]->ecs;
}


TilemapManager& SceneManager::GetTilemapManager()
{
	// TODO: Insertar una instrucción "return" aquí
	return m_scenes[m_currentSceneName]->m_tilemapManager;
}


void SceneManager::AddScene(const std::string& name, std::unique_ptr<IScene> scene)
{
	if (!m_scenes[name])
	{
		m_scenes[name] = std::move(scene);
		m_scenes[name]->ecs = std::make_unique<ECSManager>();
		if (!m_scenes[name]->ecs)
		{
			std::cout << "Error creating ECS!.." << std::endl;
		}

		m_scenes[name]->ecs->RegisterSystem<MovementSystem>(m_scenes[name]->ecs);
		m_scenes[name]->ecs->RegisterSystem<CollisionSystem>(m_scenes[name]->ecs);
		m_scenes[name]->ecs->RegisterSystem<CameraSystem>(m_scenes[name]->ecs);
		m_scenes[name]->ecs->RegisterSystem<RenderSystem>(m_scenes[name]->ecs);
		m_scenes[name]->ecs->RegisterSystem<AnimationSystem>(m_scenes[name]->ecs);

		m_currentSceneName = name;
		m_scenes[name]->OnLoad();

	}

}

void SceneManager::ChangeScene(const std::string& sceneName)
{

	if (sceneName != "mad_splash_screen_01")
	{
		if (m_nextSceneName != sceneName)
		{
			m_nextSceneName = sceneName;
		}
	}
}

void SceneManager::ChangeSceneWithTransition(const std::string& nextScene, std::unique_ptr<ISceneTransition> transitionIn, std::unique_ptr<ISceneTransition> transitionOut = nullptr)
{

	m_isSplashScreen = (nextScene == "mad_splash_screen_01");

	if (m_isSplashScreen)
	{
		m_splashSceneName = nextScene;

		m_splashTransitionIn = std::move(transitionIn);

		m_splashTransitionIn->OnStart();

		m_isTransitionRunning = false;

		if (transitionOut)
		{
			m_splashTransitionOut = std::move(transitionOut);
		}
	}
	else
	{
		if (m_nextSceneName != nextScene)
		{
			m_nextSceneName = nextScene;

			m_transitionIn = std::move(transitionIn);

			m_transitionIn->OnStart();

			m_isTransitionRunning = false;

			if (transitionOut)
			{
				m_transitionOut = std::move(transitionOut);
			}

		}
	
	}




}


void SceneManager::Update(float deltaTime)
{

	if (m_isSplashScreen)
	{
		if (m_splashTransitionIn)
		{
			m_currentTransition = std::move(m_splashTransitionIn);
		}

	}
	else
	{
		if (m_transitionIn)
		{
			m_currentTransition = std::move(m_transitionIn);
		}
	}

	if (m_currentTransition)
	{
		m_currentTransition->Update(deltaTime);

			if (!m_isTransitionRunning)
			{
				if (m_currentScene)
				{
					m_currentScene->OnSceneExit();
				}

				if (m_splashSceneName != "")
				{
					m_currentScene = m_scenes[m_splashSceneName].get();
					m_currentSceneName = m_splashSceneName;
					m_currentScene->OnSceneEnter();
					m_splashSceneName = "";
				}
				else if (m_currentSceneName != m_nextSceneName && !m_isTransitionOut)
				{
					m_currentScene = m_scenes[m_nextSceneName].get();
					m_currentSceneName = m_nextSceneName;
					m_currentScene->OnSceneEnter();
				}

				m_isTransitionRunning = true;
				
			}

			if (m_currentTransition->IsFinished() && m_currentScene &&m_currentScene->CanTransition())
			{

				m_currentTransition = nullptr;
				if (m_currentTransition)
				{
					m_currentTransition->OnEnd();
				}


				if (m_isTransitionOut)
				{
					if (m_nextSceneName != "" && m_currentSceneName != m_nextSceneName)
					{
						m_currentScene = m_scenes[m_nextSceneName].get();
						m_currentSceneName = m_nextSceneName;
						m_currentScene->OnSceneEnter();
						m_isTransitionOut = false;
					}
					m_isSplashScreen = (m_nextSceneName == "mad_splash_screen_01");
				}
				
				if (m_isSplashScreen)
				{
					if (m_splashTransitionOut)
					{
						m_currentTransition = std::move(m_splashTransitionOut);
						m_currentTransition->OnStart();
						m_isTransitionOut = true;
					}
				}
				else
				{
					if (m_transitionOut)
					{
						m_currentTransition = std::move(m_transitionOut);
						m_currentTransition->OnStart();
						m_isTransitionOut = true;

					}
				}
				

				m_isTransitionRunning = false;
			}
	}

	if (m_currentScene)
	{
		m_currentScene->Update(deltaTime);
	}
}


void SceneManager::Draw(sf::RenderWindow& window)
{
	if (m_currentScene)
	{
		m_currentScene->m_tilemapManager.Draw(window);
		m_currentScene->ecs->Draw(window);
		m_currentScene->Draw(window);
	}
	if (m_currentTransition)
	{
		m_currentTransition->Draw(window);
	}
}