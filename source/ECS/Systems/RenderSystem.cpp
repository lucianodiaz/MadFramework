#include <ECS/Systems/RenderSystem.h>
#include <ECS/Components/TransformComponent.h>
#include <ECS/Components/ColliderComponent.h>


void RenderSystem::Render(sf::RenderWindow& window)
{
	auto entities = m_ecs->GetEntitiesWithComponent<TransformComponent, SpriteComponent>();
	for (auto& entity : entities)
	{
		auto& sprite = m_ecs->GetComponent<SpriteComponent>(entity);

		window.draw(sprite.sprite);
	}

	//only for debug
	auto entitiesDebug = m_ecs->GetEntitiesWithComponent<TransformComponent, ColliderComponent>();
	for (auto& entity : entitiesDebug)
	{
		auto& transform = m_ecs->GetComponent<TransformComponent>(entity);
		auto& collider = m_ecs->GetComponent<ColliderComponent>(entity);

		if (collider.debugDraw)
		{
			auto realPosition = transform.position + collider.offset;

			if (collider.shape == ColliderShape::BOX)
			{
				sf::RectangleShape rect;
				rect.setSize(sf::Vector2f(collider.box.width, collider.box.height));
				rect.setOrigin(collider.box.width / 2.f, collider.box.height / 2.f); // otra opción
				rect.setPosition(realPosition); // ya está centrado
				rect.setFillColor(sf::Color::Transparent);
				rect.setOutlineColor(sf::Color::Red);
				rect.setOutlineThickness(1.0f);

				window.draw(rect);
			}
			else if (collider.shape == ColliderShape::CIRCLE)
			{
				sf::CircleShape circle;
				circle.setRadius(collider.circle.radius);
				circle.setOrigin(collider.circle.radius, collider.circle.radius); // centrado
				circle.setPosition(realPosition);
				circle.setFillColor(sf::Color::Transparent);
				circle.setOutlineColor(sf::Color::Red);
				circle.setOutlineThickness(1.0f);

				window.draw(circle);
			}
		}
	}
}

void RenderSystem::UpdateEntities(float deltaTime)
{
	auto entities = m_ecs->GetEntitiesWithComponent<TransformComponent, SpriteComponent>();

	for (auto& entity : entities)
	{
		auto& transform = m_ecs->GetComponent<TransformComponent>(entity);
		auto& sprite = m_ecs->GetComponent<SpriteComponent>(entity);

		sprite.sprite.setPosition(transform.position);
	}
}