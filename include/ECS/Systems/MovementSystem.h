#pragma once


#include <memory>
#include "ECS/System.h"
#include "ECS/ECSManager.h"
#include <ECS/Components/VelocityComponent.h>
#include <ECS/Components/TransformComponent.h>

class MovementSystem : public System<TransformComponent>
{
public:
	MovementSystem(std::unique_ptr<ECSManager>& ecs) : m_ecs(ecs) {
	};
protected:
	void UpdateEntities(float deltaTime) override
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

	std::unique_ptr<ECSManager>& m_ecs;
};