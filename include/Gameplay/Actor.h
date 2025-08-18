#pragma once
#include <ECS/Components/TransformComponent.h>
#include <MadFrameworkExport.h>
#include <MadFramework.h>


class World;
class ECSManager;

class MAD_API Actor
{
public:
	Actor(float x = 0, float y = 0);

	virtual ~Actor();

	Entity GetEntity() const { return m_entity; }


	virtual void Start();
	virtual void Update(float deltaTime);
	virtual void ProcessInput();


	void SetPosition(const sf::Vector2f& p);
	void SetPosition(float x, float y);

	sf::Vector2f& GetPosition();

	/* Add Component eg:  AddComponent<VelocityComponent>(50.0f,0.0f)*/
	template<typename T, typename... Args>
	T& AddComponent(Args&&... args);

	template<typename T, typename... Args>
	T& AddComponentWithName(std::string_view name, Args&&... args);

	/* Get Component of Actor eg:  GetComponent<TransformComponent>() */ 
	template<typename T>
	std::vector<T*> GetComponents();

	template<typename T>
	T& GetComponent(std::string_view name);

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
#include "Actor.inl"