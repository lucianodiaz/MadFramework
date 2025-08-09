#pragma once
#include "ECS/System.h"
#include <ECS/Components/ParticleEmitterComponent.h>
#include <ECS/ECSManager.h>



class ParticleSystem : public System<ParticleEmitterComponent>
{

public:
	ParticleSystem(std::unique_ptr<ECSManager>& ecs) : m_ecs(ecs) {}


	void Draw(sf::RenderWindow& window);
protected:

	void UpdateEntities(float deltaTime)override;

	std::unique_ptr<ECSManager>& m_ecs;
};