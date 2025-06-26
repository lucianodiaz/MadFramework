#include <ECS/Systems/CollisionSystem.h>
#include "InternalCollisionCheck.h"
#include <Core/Signal.h>
#include <Gameplay/Actor.h>
#include <Core/World.h>

void CollisionSystem::UpdateEntities(float deltaTime)
{
	auto entities = m_ecs->GetEntitiesWithComponent<ColliderComponent, TransformComponent>();

	for (size_t i = 0; i < entities.size(); ++i)
	{
		for (size_t j = i + 1; j < entities.size(); ++j)
		{
			auto& transformA = m_ecs->GetComponent<TransformComponent>(entities[i]);
			auto& colliderA = m_ecs->GetComponent<ColliderComponent>(entities[i]);
			auto& transformB = m_ecs->GetComponent<TransformComponent>(entities[j]);
			auto& colliderB = m_ecs->GetComponent<ColliderComponent>(entities[j]);

			sf::Vector2f posA = transformA.position + colliderA.offset;
			sf::Vector2f posB = transformB.position + colliderB.offset;

			bool collisionDetected = false;

			if (colliderA.shape == ColliderShape::BOX && colliderB.shape == ColliderShape::BOX)
			{
				collisionDetected = AABBColision(posA, colliderA.box.width, colliderA.box.height,
					posB, colliderB.box.width, colliderB.box.height);
			}
			else if (colliderA.shape == ColliderShape::CIRCLE && colliderB.shape == ColliderShape::CIRCLE)
			{
				collisionDetected = CircleCollision(posA, colliderA.circle.radius,
					posB, colliderB.circle.radius);
			}
			else if (colliderA.shape == ColliderShape::BOX && colliderB.shape == ColliderShape::CIRCLE)
			{
				collisionDetected = BoxCircleCollision(posA, colliderA.box.width, colliderA.box.height,
					posB, colliderB.circle.radius);
			}

			if (collisionDetected)
			{
				auto actorA = World::GetWorld()->GetActor(entities[i]);
				auto actorB = World::GetWorld()->GetActor(entities[j]);

				if (colliderA.isTrigger || colliderB.isTrigger)
				{
					Signal::GetInstance().Dispatch<Actor*, Actor*>("onTriggeredDetected", &actorA, &actorB);
					Signal::GetInstance().Dispatch<Actor*, Actor*>("onTriggeredDetected", &actorB, &actorA);
				}
				else
				{
					Signal::GetInstance().Dispatch<Actor*, Actor*>("onCollisionDetected", &actorA, &actorB);
					Signal::GetInstance().Dispatch<Actor*, Actor*>("onCollisionDetected", &actorB, &actorA);
				}
			}
		}
	}
}
