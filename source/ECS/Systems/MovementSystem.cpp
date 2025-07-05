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

			auto nextPosition = transform.position + velocityComponent.velocity * deltaTime;

			transform.isDirty = nextPosition != transform.position; // Check if the position has changed

			transform.position = nextPosition;
		}

	}
}
