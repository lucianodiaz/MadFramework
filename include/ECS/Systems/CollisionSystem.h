#pragma once

#include <Core/QuadTree.h>
#include "ECS/System.h"
#include "ECS/ECSManager.h"
#include <ECS/Components/ColliderComponent.h>
#include <ECS/Components/TransformComponent.h>
#include <unordered_set>

struct PairHash {
	std::size_t operator()(const std::pair<Entity, Entity>& p) const {
		return std::hash<Entity>{}(p.first) ^ std::hash<Entity>{}(p.second);
	}
};

class CollisionSystem : public System<ColliderComponent>
{
public:
	CollisionSystem(std::unique_ptr<ECSManager>& ecs);

protected:

	void UpdateEntities(float deltaTime) override;


	std::unordered_set<Entity> m_currentCollision;
	std::unique_ptr<ECSManager>& m_ecs;
	std::unique_ptr<QuadTree> m_quadTree;
};