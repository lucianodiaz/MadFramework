#include <ECS/ECSManager.h>
#include <ECS/Systems/RenderSystem.h>
#include <ECS/Components/ColliderComponent.h>
#include <ECS/Components/SpriteAnimationComponent.h>
#include <ECS/Components/CameraViewComponent.h>
#include <ECS/Components/TransformComponent.h>




RenderSystem::RenderSystem(std::unique_ptr<ECSManager>& ecs) : m_ecs(ecs)
{

}

void RenderSystem::Render(sf::RenderWindow& window)
{
	
    // Sprites
    for (auto e : m_ecs->GetEntitiesWithComponent<TransformComponent, SpriteComponent>())
        for (auto* s : m_ecs->GetComponents<SpriteComponent>(e))
            window.draw(s->sprite);

    // Animations
    for (auto e : m_ecs->GetEntitiesWithComponent<TransformComponent, SpriteAnimationComponent>())
        for (auto* a : m_ecs->GetComponents<SpriteAnimationComponent>(e))
            if (!a->animations.empty()) window.draw(a->sprite);

    // Debug colliders
    for (auto e : m_ecs->GetEntitiesWithComponent<TransformComponent, ColliderComponent>())
    {
        auto& tr = m_ecs->GetComponent<TransformComponent>(e);
        for (auto* c : m_ecs->GetComponents<ColliderComponent>(e))
        {
            if (!c->debugDraw) continue;
            auto realPosition = tr.position + c->offset;

            if (c->shape == ColliderShape::BOX) {
                sf::RectangleShape rect;
                rect.setSize({ c->box.width, c->box.height });
                rect.setOrigin(c->box.width / 2.f, c->box.height / 2.f);
                rect.setPosition(realPosition);
                rect.setFillColor(sf::Color(c->debugColor.r, c->debugColor.g, c->debugColor.b, 60));
                rect.setOutlineColor(c->debugColor);
                rect.setOutlineThickness(1.f);
                window.draw(rect);
            }
            else if (c->shape == ColliderShape::CIRCLE) {
                sf::CircleShape circle;
                circle.setRadius(c->circle.radius);
                circle.setOrigin(c->circle.radius, c->circle.radius);
                circle.setPosition(realPosition);
                circle.setFillColor(sf::Color::Transparent);
                circle.setOutlineColor(c->debugColor);
                circle.setOutlineThickness(1.f);
                window.draw(circle);
            }
            else if (c->shape == ColliderShape::POLYGON) {
                sf::ConvexShape poly;
                poly.setPointCount(c->polygon.points.size());
                for (size_t i = 0; i < c->polygon.points.size(); ++i)
                    poly.setPoint(i, c->polygon.points[i] + realPosition);
                poly.setFillColor(sf::Color::Transparent);
                poly.setOutlineColor(c->debugColor);
                poly.setOutlineThickness(1.f);
                window.draw(poly);
            }
        }
    }

}

void RenderSystem::UpdateEntities(float deltaTime)
{
	// Sprites
	for (auto e : m_ecs->GetEntitiesWithComponent<TransformComponent, SpriteComponent>())
	{
		auto& tr = m_ecs->GetComponent<TransformComponent>(e);
		for (auto* s : m_ecs->GetComponents<SpriteComponent>(e))
			s->sprite.setPosition(tr.position);
	}

	// Animations
	for (auto e : m_ecs->GetEntitiesWithComponent<TransformComponent, SpriteAnimationComponent>())
	{
		auto& tr = m_ecs->GetComponent<TransformComponent>(e);
		for (auto* a : m_ecs->GetComponents<SpriteAnimationComponent>(e))
			a->sprite.setPosition(tr.position);
	}
}