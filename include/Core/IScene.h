#pragma once
#include <SFML/Graphics.hpp>
#include <ECS/ECSManager.h>
#include <Core/TilemapManager.h>


class IScene
{
public:
	IScene() {};

	virtual void OnLoad() = 0;
	virtual void OnUnload() = 0;

	virtual void OnSceneEnter() = 0;
	virtual void OnSceneExit() = 0;

	virtual void Update(float deltaTime) = 0;
	virtual void Draw(sf::RenderWindow& window) = 0;


	virtual bool CanTransition() const = 0;

	virtual ~IScene() {};

	friend class SceneManager;
private:

	std::unique_ptr<ECSManager> ecs;
	TilemapManager m_tilemapManager;


};