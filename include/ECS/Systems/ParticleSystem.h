#pragma once
#include "ECS/System.h"
#include <ECS/Components/ParticleEmitterComponent.h>


class ECSManager;


class ParticleSystem : public System<ParticleEmitterComponent>
{

public:
	ParticleSystem(std::unique_ptr<ECSManager>& ecs);


	void Render(sf::RenderWindow& window)override;
protected:

	void UpdateEntities(float deltaTime)override;

	std::unique_ptr<ECSManager>& m_ecs;
};