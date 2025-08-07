#pragma once
#include <ECS/System.h>
#include <ECS/Components/CameraViewComponent.h>
#include <ECS/ECSManager.h>

class CameraSystem : public System<CameraViewComponent>
{

public:
	CameraSystem(std::unique_ptr<ECSManager>& ecs) : m_ecs(ecs) {};

protected:

	sf::View& GetLetterBoxView(sf::View view);

	void UpdateEntities(float deltaTime) override;

	void UpdateShake(float deltaTime, CameraViewComponent& cam);

	void ClampToBounds(CameraViewComponent& cam);

	std::unique_ptr<ECSManager>& m_ecs;
};