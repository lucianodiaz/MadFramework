#pragma once
#include <memory>
#include "ECS/System.h"
#include "ECS/ECSManager.h"
#include "ECS/Components/TransformComponent.h"

class MovementSystem : public System<TransformComponent>
{
public:
	MovementSystem(std::unique_ptr<ECSManager>& ecs) : m_ecs(ecs) {
	};
protected:
	void UpdateEntities(float deltaTime) override;

	std::unique_ptr<ECSManager>& m_ecs;
};