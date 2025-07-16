#include "ECS/ECSManager.h"
#include <ECS/Systems/RenderSystem.h>

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

void ECSManager::Draw(sf::RenderWindow& window)
{
    for (auto& system : m_systems)
    {
        // Intentar castear a RenderSystem
        auto renderSystem = std::dynamic_pointer_cast<RenderSystem>(system);
        if (renderSystem)
        {
            renderSystem->Render(window);
        }
    }
}

std::vector<Entity> ECSManager::GetEntities()
{
    return m_entities;
}
