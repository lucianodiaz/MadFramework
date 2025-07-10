#include <ECS/Systems/CollisionSystem.h>
#include "InternalCollisionCheck.h"
#include <Core/Signal.h>
#include <Gameplay/Actor.h>
#include <Core/World.h>

CollisionSystem::CollisionSystem(std::unique_ptr<ECSManager>& ecs) : m_ecs(ecs)
{
	m_quadTree = std::make_unique<QuadTree>(0, sf::FloatRect(0, 0, 800,600), m_ecs);
}

void CollisionSystem::UpdateEntities(float deltaTime)
{
	auto width = World::GetWorld()->GetWindow().GetRenderWindow().getSize().x;
	auto height = World::GetWorld()->GetWindow().GetRenderWindow().getSize().y;
	
	m_quadTree->SetBounds( sf::FloatRect(0, 0, width, height)); // Update the bounds of the QuadTree to match the window size

	auto entities = m_ecs->GetEntitiesWithComponent<ColliderComponent, TransformComponent>();

	auto& previousCollisions = m_currentCollision;

	m_currentCollision.clear();

	m_quadTree->Clear();

	for (const auto& entity : entities)
	{
		auto& transform = m_ecs->GetComponent<TransformComponent>(entity);
		if (transform.isDirty)
		{
			m_quadTree->Insert(entity);
			transform.isDirty = false;
		}
	}

	std::unordered_set<std::pair<Entity, Entity>, PairHash> checkedPairs;

	for (auto& entityA : entities)
	{
		auto nearby = m_quadTree->Retrieve(entityA);

		for (Entity entityB : nearby)
		{
			if (entityA == entityB) continue;

			Entity minID = std::min(entityA, entityB);
			Entity maxID = std::max(entityA, entityB);

			std::pair<Entity, Entity> pairkKey = { minID, maxID };

			if (checkedPairs.contains(pairkKey)) continue;

			checkedPairs.insert(pairkKey);
			
			auto actorA = World::GetWorld()->GetActor(entityA);
			auto actorB = World::GetWorld()->GetActor(entityB);

			if (actorA.GetGameTag() == actorB.GetGameTag()) continue;


			auto& transformA = m_ecs->GetComponent<TransformComponent>(entityA);
			auto& colliderA = m_ecs->GetComponent<ColliderComponent>(entityA);
			auto& transformB = m_ecs->GetComponent<TransformComponent>(entityB);
			auto& colliderB = m_ecs->GetComponent<ColliderComponent>(entityB);

			sf::Vector2f posA = transformA.position + colliderA.offset;
			sf::Vector2f posB = transformB.position + colliderB.offset;
			sf::Vector2f mtv;

			bool collisionDetected = false;

		/*	if (colliderA.shape == ColliderShape::BOX && colliderB.shape == ColliderShape::BOX)
			{
				collisionDetected = MAD::CollisionUtils::AABBCollision(posA, colliderA.box.width, colliderA.box.height,
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
			}*/

			//We'll gonna use SAT Collision for every shape whiout cirlces, circles will be handled separately

			if (colliderA.shape == ColliderShape::CIRCLE && colliderB.shape == ColliderShape::CIRCLE)
			{
				collisionDetected = MAD::CollisionUtils::CircleCollision(posA, colliderA.circle.radius,
					posB, colliderB.circle.radius,mtv);
			}
			else if(colliderA.shape == ColliderShape::CIRCLE && colliderB.shape == ColliderShape::POLYGON)
			{
				collisionDetected = MAD::CollisionUtils::CirclePolygonCollision(colliderA, transformA, colliderB, transformB, mtv);
			}
			else if (colliderA.shape == ColliderShape::POLYGON && colliderB.shape == ColliderShape::CIRCLE)
			{
				collisionDetected = MAD::CollisionUtils::CirclePolygonCollision(colliderB, transformB, colliderA, transformA, mtv);
			}
			else
			{
				collisionDetected = MAD::CollisionUtils::SATCollision(colliderA, transformA, colliderB, transformB, mtv);
			}

			if (collisionDetected)
			{

				m_currentCollision.insert(pairkKey);

			
				Signal::GetInstance().Dispatch<Actor*, Actor*>("onCollisionDetected", &actorA, &actorB);
				Signal::GetInstance().Dispatch<Actor*, Actor*>("onCollisionDetected", &actorB, &actorA);

				//Stop Entity when is colliding with something

				if(!colliderA.isStatic && (!colliderA.isTrigger && !colliderB.isTrigger)) transformA.position += mtv;
				
				if(!colliderB.isStatic && (!colliderB.isTrigger && !colliderA.isTrigger)) transformB.position -= mtv;

			}
		}
	}

}
