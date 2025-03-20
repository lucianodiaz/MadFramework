#include "ECS/System.h"
#include <ECS/ECSManager.h>
#include "ECS/Components/SpriteComponent.h"
#include <ECS/Components/TransformComponent.h>


class RenderSystem : public System<SpriteComponent>
{
public:
	RenderSystem(std::unique_ptr<ECSManager>& ecs) : m_ecs(ecs) 
	{
	};

	void Render(sf::RenderWindow& window)
	{
		auto entities = m_ecs->GetEntitiesWithComponent<TransformComponent, SpriteComponent>();
		for (auto& entity : entities)
		{
			auto& sprite = m_ecs->GetComponent<SpriteComponent>(entity);

			window.draw(sprite.sprite);
		}
	}
	

protected:

	void UpdateEntities(float deltaTime)override
	{
		auto entities = m_ecs->GetEntitiesWithComponent<TransformComponent,SpriteComponent>();

		for (auto& entity : entities)
		{
			auto& transform = m_ecs->GetComponent<TransformComponent>(entity);
			auto& sprite = m_ecs->GetComponent<SpriteComponent>(entity);

			sprite.sprite.setPosition(transform.position);
		}
	}

	std::unique_ptr<ECSManager>& m_ecs;
};