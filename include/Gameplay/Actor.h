#pragma once

#include <Core/World.h>
#include <ECS/ECSManager.h>
#include <ECS/Components/TransformComponent.h>
#include <MadFrameworkExport.h>
#include <MadFramework.h>



class MAD_API Actor
{
public:
	Actor(float x = 0, float y = 0);

	virtual ~Actor();

	Entity GetEntity() const { return m_entity; }


	virtual void Start();
	virtual void Update(float deltaTime);
	virtual void ProcessInput();


	template<typename ...Args>
	void SetPosition(Args& ... args);

	sf::Vector2f& GetPosition();

	/* Add Component eg:  AddComponent<VelocityComponent>(50.0f,0.0f)*/
	template<typename T, typename... Args>
	void AddComponent(Args&&... args);

	/* Get Component of Actor eg:  GetComponent<TransformComponent>() */ 
	template<typename T>
	T& GetComponent();

	/* Get GameTag of Actor is usseful to check things */
	const std::string GetGameTag() const { return m_tag; }

	void SetGameTag(const std::string& tag) { m_tag = tag; }

protected:

	ECSManager& m_ecs;
	Entity m_entity;

	std::string m_tag= "Actor";

	virtual void onCollisionEnter(Actor& actorA, Actor& actorB);
	virtual void onCollisionExit(Actor& actorA);


	friend class World;

private:

	void AddSignalListeners();
};

template<typename ...Args>
inline void Actor::SetPosition(Args & ...args)
{
	m_ecs.GetComponent<TransformComponent>(m_entity).setPosition(std::forward<Args>(args)...);
}

template<typename T, typename ...Args>
inline void Actor::AddComponent(Args && ...args)
{
	m_ecs.AddComponent<T>(m_entity, std::forward<Args>(args)...);
}

template<typename T>
inline T& Actor::GetComponent()
{
	return m_ecs.GetComponent<T>(m_entity);	
}
