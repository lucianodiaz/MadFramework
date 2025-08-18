#include <ECS/Systems/MovementSystem.h>
#include <ECS/Components/VelocityComponent.h>
#include <ECS/ECSManager.h>

MovementSystem::MovementSystem(std::unique_ptr<ECSManager>& ecs) : m_ecs(ecs)
{
}

void MovementSystem::UpdateEntities(float deltaTime)
{
	auto entities = m_ecs->GetEntitiesWithComponent<TransformComponent>();

	for (auto& entity : entities)
	{
		auto& transform = m_ecs->GetComponent<TransformComponent>(entity);
		
		if (m_ecs->HasComponent<VelocityComponent>(entity))
		{
			auto& velocityComponent = m_ecs->GetComponent<VelocityComponent>(entity);

			sf::Vector2f dir{ velocityComponent.velocity.x, velocityComponent.velocity.y };

			float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

			if (length != 0.0f)
			{
				dir /= length;
			}
			velocityComponent.velocity = dir; // Normalize the velocity vector

			auto nextPosition = transform.position + (velocityComponent.velocity * velocityComponent.speed) * deltaTime;

			transform.isDirty = nextPosition != transform.position; // Check if the position has changed

			transform.position = nextPosition;
		}

	}
}
