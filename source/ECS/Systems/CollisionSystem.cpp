#include <ECS/Systems/CollisionSystem.h>
#include "InternalCollisionCheck.h"
#include <Core/Signal.h>
#include <Gameplay/Actor.h>
#include <Core/World.h>

void CollisionSystem::UpdateEntities(float deltaTime)
{
	auto entities = m_ecs->GetEntitiesWithComponent<ColliderComponent, TransformComponent>();

	if (entities.size() < 2) return; // No need to check collisions if less than 2 entities
	for (size_t i = 0; i < 70; ++i)
	{
		for (size_t j = i + 1; j < 70; ++j)
		{
			auto actorA = World::GetWorld()->GetActor(entities[i]);
			auto actorB = World::GetWorld()->GetActor(entities[j]);

			if (actorA.GetGameTag() == actorB.GetGameTag()) continue;
			auto& transformA = m_ecs->GetComponent<TransformComponent>(entities[i]);
			auto& colliderA = m_ecs->GetComponent<ColliderComponent>(entities[i]);
			auto& transformB = m_ecs->GetComponent<TransformComponent>(entities[j]);
			auto& colliderB = m_ecs->GetComponent<ColliderComponent>(entities[j]);

			

			sf::Vector2f posA = transformA.position + colliderA.offset;
			sf::Vector2f posB = transformB.position + colliderB.offset;

			bool collisionDetected = false;

			std::unordered_set<Entity>& previousCollisions = m_currentCollision;

			m_currentCollision.clear();

			if (colliderA.shape == ColliderShape::BOX && colliderB.shape == ColliderShape::BOX)
			{
				collisionDetected = MAD::CollisionUtils::AABBColision(posA, colliderA.box.width, colliderA.box.height,
					posB, colliderB.box.width, colliderB.box.height);
			}
			else if (colliderA.shape == ColliderShape::CIRCLE && colliderB.shape == ColliderShape::CIRCLE)
			{
				collisionDetected = MAD::CollisionUtils::CircleCollision(posA, colliderA.circle.radius,
					posB, colliderB.circle.radius);
			}
			else if (colliderA.shape == ColliderShape::BOX && colliderB.shape == ColliderShape::CIRCLE)
			{
				collisionDetected = MAD::CollisionUtils::BoxCircleCollision(posA, colliderA.box.width, colliderA.box.height,
					posB, colliderB.circle.radius);
			}

			if (collisionDetected)
			{
				

				m_currentCollision.insert(entities[i]);
				m_currentCollision.insert(entities[j]);

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

			for (const auto& prevCollision : previousCollisions)
			{

				auto actorA = World::GetWorld()->GetActor(entities[i]);
				auto actorB = World::GetWorld()->GetActor(entities[j]);

				if (m_currentCollision.find(prevCollision) == m_currentCollision.end())
				{
					Signal::GetInstance().Dispatch<Actor*>("onCollisionEnded", &actorA);
					Signal::GetInstance().Dispatch<Actor*>("onCollisionEnded", &actorB);
				}
			}
		}
	}


}
