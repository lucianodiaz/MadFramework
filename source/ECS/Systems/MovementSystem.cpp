#include <ECS/Systems/MovementSystem.h>
#include <ECS/Components/VelocityComponent.h>

void MovementSystem::UpdateEntities(float deltaTime)
{
	auto entities = m_ecs->GetEntitiesWithComponent<TransformComponent>();

	for (auto& entity : entities)
	{
		auto& transform = m_ecs->GetComponent<TransformComponent>(entity);
		if (m_ecs->HasComponent<VelocityComponent>(entity))
		{
			auto& velocityComponent = m_ecs->GetComponent<VelocityComponent>(entity);

			transform.position.x += velocityComponent.velocity.x * deltaTime;
			transform.position.y += velocityComponent.velocity.y * deltaTime;
		}

	}
}
