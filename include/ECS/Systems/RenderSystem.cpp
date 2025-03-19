#include "ECS/System.h"
#include "ECS/Components/SpriteComponent.h"
#include <ECS/ECSManager.h>
#include <ECS/Components/TransformComponent.h>


class RenderSystem : public System<SpriteComponent>
{
public:
	RenderSystem(std::unique_ptr<ECSManager>& ecs) : m_ecs(ecs) 
	{
	};
	

protected:

	void UpdateEntities(float deltaTime)override
	{
		auto entities = m_ecs->GetEntitiesWithComponent<TransformComponent,SpriteComponent>();
	}

	std::unique_ptr<ECSManager>& m_ecs;
};