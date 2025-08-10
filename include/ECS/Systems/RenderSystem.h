#include "ECS/System.h"
#include <ECS/ECSManager.h>
#include <ECS/Components/SpriteComponent.h>
#include <ECS/Components/TransformComponent.h>
#include <Core/Signal.h>
#include <Core/World.h>

class RenderSystem : public System<SpriteComponent>
{
public:
	RenderSystem(std::unique_ptr<ECSManager>& ecs);

	void Render(sf::RenderWindow& window);

protected:

	void UpdateEntities(float deltaTime)override;
	std::unique_ptr<ECSManager>& m_ecs;

};