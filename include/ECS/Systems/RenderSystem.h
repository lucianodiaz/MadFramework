#include "ECS/System.h"
#include <ECS/Components/SpriteComponent.h>


class ECSManager;
class World;


class RenderSystem : public System<SpriteComponent>
{
public:
	RenderSystem(std::unique_ptr<ECSManager>& ecs);

	void Render(sf::RenderWindow& window)override;

protected:

	void UpdateEntities(float deltaTime)override;
	std::unique_ptr<ECSManager>& m_ecs;

};