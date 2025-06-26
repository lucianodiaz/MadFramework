#pragma once

#include <ECS/ECSManager.h>
#include <MadFrameworkExport.h>


class MAD_API Actor
{
public:
	Actor(float x = 0, float y = 0);

	virtual ~Actor();

	Entity GetEntity() const { return m_entity; }


	virtual void Start();
	virtual void Update(float deltaTime);
	virtual void ProcessInput();

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
	virtual void onTriggerEnter(Actor& actorA, Actor& actorB);


	friend class World;

private:

	void AddSignalListeners();
};

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
