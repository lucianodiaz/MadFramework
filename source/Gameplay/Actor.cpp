#include "Gameplay/Actor.h"
#include <ECS/Components/TransformComponent.h>
#include <Core/World.h>
#include <MadFramework.h>



Actor::Actor(float x, float y) : m_ecs(*World::GetWorld()->GetECSManager())
{	
	m_entity = m_ecs.CreateEntity();

	m_ecs.AddComponent<TransformComponent>(m_entity, x, y);

    AddSignalListeners();
}

Actor::~Actor()
{
}

void Actor::Start()
{
	std::cout << m_tag <<" Start" << std::endl;

   
}

void Actor::Update(float deltaTime)
{
}

void Actor::ProcessInput()
{
}

void Actor::onCollisionEnter(Actor& actorA, Actor& actorB)
{
}

void Actor::onCollisionExit(Actor& actorA)
{
}

void Actor::AddSignalListeners()
{
    Signal::GetInstance().AddListener<Actor*, Actor*>("onCollisionDetected",
        static_cast<std::function<void(Actor*, Actor*)>>([this](Actor* actorA, Actor* actorB)
            {
                if (actorA->GetEntity() == m_entity || actorB->GetEntity() == m_entity)
                {
                    std::cout << "Collision detected with " << (actorA->GetEntity() == m_entity ? actorB->GetGameTag() : actorA->GetGameTag()) << std::endl;

                    onCollisionEnter(*actorA, *actorB);
                }
            }));

    Signal::GetInstance().AddListener<Actor*>("onCollisionEnded",
        static_cast<std::function<void(Actor*)>>([this](Actor* actorA)
            {
              
                    std::cout << "Collision ended with " << (actorA->GetGameTag()) << std::endl;

                    onCollisionExit(*actorA);
                
            }));
}
