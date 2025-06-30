#pragma once
#include "ECS/System.h"
#include "ECS/ECSManager.h"
#include <ECS/Components/ColliderComponent.h>
#include <ECS/Components/TransformComponent.h>
#include <unordered_set>

class CollisionSystem : public System<ColliderComponent>
{
public:
	CollisionSystem(std::unique_ptr<ECSManager>& ecs) : m_ecs(ecs)
	{
	};

protected:

	void UpdateEntities(float deltaTime) override;


	std::unordered_set<ColliderComponent> m_currentCollision;
	std::unique_ptr<ECSManager>& m_ecs;
};