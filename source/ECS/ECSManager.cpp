#include "ECS/ECSManager.h"
#include <ECS/Systems/RenderSystem.h>
#include <ECS/Systems/ParticleSystem.h>
#include <SFML/Graphics.hpp>

Entity ECSManager::CreateEntity()
{
    Entity entity = m_entities.size();

    m_entities.emplace_back(entity);

    m_entityComponentMasks[entity].reset();

    return entity;
}

void ECSManager::DestroyEntity(Entity entity)
{
    m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), entity), m_entities.end());
    m_componentsMulti.erase(entity);
    m_nameIndex.erase(entity);
    m_entityComponentMasks.erase(entity);
}

void ECSManager::Update(float deltaTime)
{
    for (auto& system : m_systems)
    {
        system->Update(deltaTime);
    }
}

void ECSManager::Draw(sf::RenderWindow& window)
{
    for (auto& system : m_systems)
    {
		system->Render(window);
    }
}

std::vector<Entity> ECSManager::GetEntities()
{
    return m_entities;
}
