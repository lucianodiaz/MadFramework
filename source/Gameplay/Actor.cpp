#include "Gameplay/Actor.h"
#include <ECS/Components/TransformComponent.h>
#include <Core/World.h>



Actor::Actor(float x, float y) : m_ecs(*World::GetWorld()->GetECSManager())
{	
	m_entity = m_ecs.CreateEntity();

	m_ecs.AddComponent<TransformComponent>(m_entity, x, y);
}

Actor::~Actor()
{
}
