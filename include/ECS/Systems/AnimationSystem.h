#pragma once
#include "ECS/System.h"
#include <ECS/Components/SpriteAnimationComponent.h>
#include <ECS/ECSManager.h>


class AnimationSystem : public System<SpriteAnimationComponent>
{

public:
	AnimationSystem(std::unique_ptr<ECSManager>& ecs) : m_ecs(ecs) {};


protected:

	void UpdateEntities(float deltaTime) override;

	std::unique_ptr<ECSManager>& m_ecs;

};