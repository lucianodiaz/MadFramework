#pragma once
#include <SFML/Graphics.hpp>
#include <ECS/ECSManager.h>
#include <Core/TilemapManager.h>


class IScene
{
public:
	IScene() {};

	/* this is call when you add a new Scene this call only once*/
	virtual void OnLoad() = 0;

	virtual void OnUnload() = 0;

	/*this call every time you enter in scene in game engines this is call "OnBegin" "Start"*/
	virtual void OnSceneEnter() = 0;

	/*this is call every time you change scene*/
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