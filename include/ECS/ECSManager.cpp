#include "ECSManager.h"

Entity ECSManager::CreateEntity()
{
    Entity entity = m_entities.size();

    m_entities.push_back(entity);

    m_entityComponentMasks[entity].reset();

    return entity;
}

void ECSManager::DestroyEntity(Entity entity)
{
    m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), entity), m_entities.end());
    m_entityComponentMasks.erase(entity);
    m_components.erase(entity);
}

void ECSManager::Update(float deltaTime)
{
    for (auto& system : m_systems)
    {
        system->Update(deltaTime);
    }
}

std::vector<Entity> ECSManager::GetEntities()
{
    return m_entities;
}
