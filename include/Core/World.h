#pragma once
#include <SFML/Graphics.hpp>
#include <Gameplay/Actor.h>
#include <Window/Window.h>


class ECSManager;

class World
{
public:

	virtual ~World();

	void Run(int frame_per_seconds = 30);

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

protected:
	World();

private:
	

	void CreateMainWindow(int width,int height, std::string name);

	void CreateECSManager();

	void RegisterDefaultSystems();

	void ProcessInput();

	void Update(float deltaTime);

	void Render();

	std::unique_ptr<Window> _window;

	std::unique_ptr<ECSManager> ecs;

	bool m_isRunning;

	std::vector<std::unique_ptr<Actor>> m_actors;


	static std::shared_ptr <World> _world;

	friend class Actor;
	friend class ISystem;
};


template<typename T, typename ...Args>
inline T& World::SpawnActor(Args && ...args)
{
	static_assert(std::is_base_of<Actor, T>::value, "T must derive from Actor");

	m_actors.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));

	return static_cast<T&>(*m_actors.back());
}
