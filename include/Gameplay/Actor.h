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

	template<typename T, typename... Args>
	void AddComponent(Args&&... args);

	template<typename T>
	T& GetComponent();


protected:

	ECSManager& m_ecs;
	Entity m_entity;



	friend class World;
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
