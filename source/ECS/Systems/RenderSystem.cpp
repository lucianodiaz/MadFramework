#include <ECS/Systems/RenderSystem.h>
#include <ECS/Components/TransformComponent.h>
#include <ECS/Components/ColliderComponent.h>
#include <ECS/Components/SpriteAnimationComponent.h>
#include <ECS/Components/CameraViewComponent.h>




RenderSystem::RenderSystem(std::unique_ptr<ECSManager>& ecs) : m_ecs(ecs)
{
	
		Signal::GetInstance().AddListener
		(
			"onFPSUpdate",
			static_cast<std::function<void(int)>>([this](int fps)
				{
					fpsText.setString("FPS: " +  std::to_string(fps));
				})
		);
}

void RenderSystem::Render(sf::RenderWindow& window)
{
	auto entities = m_ecs->GetEntitiesWithComponent<TransformComponent, SpriteComponent>();

	for (auto& entity : entities)
	{
		auto& sprite = m_ecs->GetComponent<SpriteComponent>(entity);

		window.draw(sprite.sprite);
	}

	auto entitiesAnimated = m_ecs->GetEntitiesWithComponent<TransformComponent, SpriteAnimationComponent>();

	for (auto& entity : entitiesAnimated)
	{
		auto& animation = m_ecs->GetComponent<SpriteAnimationComponent>(entity);
		if (animation.animations.empty())
			continue;
		auto& animationData = animation.animations[animation.currentAnimation];
		window.draw(animation.sprite);
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
				rect.setFillColor(sf::Color::Color(collider.debugColor.r, collider.debugColor.g, collider.debugColor.b,60));
				rect.setOutlineColor(collider.debugColor);
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
				circle.setOutlineColor(collider.debugColor);
				circle.setOutlineThickness(1.0f);

				window.draw(circle);
			}
		}
	}


	
	if (World::GetWorld()->IsShowFPS())
	{
		fpsText.setFont(World::GetWorld()->GetFont("default"));
		fpsText.setCharacterSize(24);
		fpsText.setFillColor(sf::Color::White);

		for (auto entity : m_ecs->GetEntitiesWithComponent<CameraViewComponent>())
		{
			auto camera = m_ecs->GetComponent<CameraViewComponent>(entity);
			sf::Vector2f center = camera.cameraView.getCenter();
			sf::Vector2f size = camera.cameraView.getSize();
			fpsText.setPosition((center.x + size.x / 2.f) - (fpsText.getGlobalBounds().width *1.2), (center.y - size.y / 2.f) - (fpsText.getGlobalBounds().height/2)+10.f);
		}
		window.draw(fpsText);
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

	auto entitiesAnimated = m_ecs->GetEntitiesWithComponent<TransformComponent, SpriteAnimationComponent>();

	for (auto& entity : entitiesAnimated)
	{
		auto& transform = m_ecs->GetComponent<TransformComponent>(entity);
		auto& animation = m_ecs->GetComponent<SpriteAnimationComponent>(entity);

		if (animation.animations.empty())
			continue;

		animation.sprite.setPosition(transform.position);
	}
}