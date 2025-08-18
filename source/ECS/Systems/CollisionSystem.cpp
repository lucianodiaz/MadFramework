#include <ECS/Systems/CollisionSystem.h>
#include "InternalCollisionCheck.h"
#include <Core/Signal.h>
#include <Gameplay/Actor.h>
#include <ECS/ECSManager.h>
#include <Core/World.h>
#include <Window/Window.h>

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
            std::pair<Entity, Entity> pairKey = { minID, maxID };
            if (checkedPairs.contains(pairKey)) continue;
            checkedPairs.insert(pairKey);

            auto actorA = World::GetWorld()->GetActor(entityA);
            auto actorB = World::GetWorld()->GetActor(entityB);
            if (actorA.GetGameTag() == actorB.GetGameTag()) continue;

            auto& transformA = m_ecs->GetComponent<TransformComponent>(entityA);
            auto& transformB = m_ecs->GetComponent<TransformComponent>(entityB);


            auto collidersA = m_ecs->GetComponents<ColliderComponent>(entityA);
            auto collidersB = m_ecs->GetComponents<ColliderComponent>(entityB);
            if (collidersA.empty() || collidersB.empty()) continue;

            for (auto* colA : collidersA)
            {
                for (auto* colB : collidersB)
                {
                    sf::Vector2f posA = transformA.position + colA->offset;
                    sf::Vector2f posB = transformB.position + colB->offset;
                    sf::Vector2f mtv;
                    bool collisionDetected = false;

                    // mismas ramas que ya tenías, pero con colA/colB
                    if (colA->shape == ColliderShape::CIRCLE && colB->shape == ColliderShape::CIRCLE)
                    {
                        collisionDetected = MAD::CollisionUtils::CircleCollision(posA, colA->circle.radius,
                            posB, colB->circle.radius, mtv);
                    }
                    else if (colA->shape == ColliderShape::CIRCLE && colB->shape == ColliderShape::POLYGON)
                    {
                        collisionDetected = MAD::CollisionUtils::CirclePolygonCollision(*colA, transformA, *colB, transformB, mtv);
                    }
                    else if (colA->shape == ColliderShape::POLYGON && colB->shape == ColliderShape::CIRCLE)
                    {
                        collisionDetected = MAD::CollisionUtils::CirclePolygonCollision(*colB, transformB, *colA, transformA, mtv);
                        mtv = -mtv;
                    }
                    else
                    {
                        collisionDetected = MAD::CollisionUtils::SATCollision(*colA, transformA, *colB, transformB, mtv);
                    }

                    if (!collisionDetected) continue;

                    // Señal por colisión (puedes incluir nombres si quieres)
                    m_currentCollision.insert(pairKey);
                    Signal::GetInstance().Dispatch<Actor*, Actor*>("onCollisionDetected", &actorA, &actorB);
                    Signal::GetInstance().Dispatch<Actor*, Actor*>("onCollisionDetected", &actorB, &actorA);

                    // Resolución física/trigger por instancia
                    if (!colA->isTrigger && !colB->isTrigger)
                    {
                        if (!colA->isStatic && !colB->isStatic)
                        {
                            transformA.position -= mtv * 0.5f;
                            transformB.position += mtv * 0.5f;
                            transformA.isDirty = transformB.isDirty = true;
                        }
                        else if (!colA->isStatic && colB->isStatic)
                        {
                            transformA.position -= mtv;
                            if (m_ecs->HasComponent<VelocityComponent>(entityA)) {
                                auto& vA = m_ecs->GetComponent<VelocityComponent>(entityA);
                                if (std::abs(mtv.x) > std::abs(mtv.y)) vA.velocity.x = 0.0f; else vA.velocity.y = 0.0f;
                            }
                        }
                        else if (!colB->isStatic && colA->isStatic)
                        {
                            transformB.position += mtv;
                            if (m_ecs->HasComponent<VelocityComponent>(entityB)) {
                                auto& vB = m_ecs->GetComponent<VelocityComponent>(entityB);
                                if (std::abs(mtv.x) > std::abs(mtv.y)) vB.velocity.x = 0.0f; else vB.velocity.y = 0.0f;
                            }
                        }
                    }
                }
            }
        }
	}

}
