#pragma once
#include "Component.h"
#include "Entity.h"
#include "System.h"
#include <bitset>
#include <memory>
#include <unordered_map>
#include <vector>
#include <SFML/Graphics.hpp>




constexpr size_t MAX_COMPONENTS = 32;

class ECSManager
{
public:
	ECSManager() = default;

	Entity CreateEntity();


	void DestroyEntity(Entity entity);

	template<typename T, typename... Args>
	void AddComponent(Entity entity, Args&&... args);

	template<typename T>
	T& GetComponent(Entity entity);

	template<typename T>
	bool HasComponent(Entity entity);

	template<typename T>
	void RemoveComponent(Entity entity);

	template<typename T, typename... Args>
	void RegisterSystem(Args&&... args);

	void Update(float deltaTime);

	void Draw(sf::RenderWindow& window);

	std::vector<std::shared_ptr<ISystem>>& GetSystems() {return m_systems; }

	std::vector<Entity> GetEntities();

	template<typename... Components>
	std::vector<Entity> GetEntitiesWithComponent();


private:

	std::vector<Entity> m_entities;
	std::unordered_map<Entity, std::bitset<MAX_COMPONENTS>> m_entityComponentMasks;
	std::unordered_map < Entity, std::unordered_map<size_t, std::unique_ptr<IComponent>>> m_components;
	std::vector<std::shared_ptr<ISystem>> m_systems;
};

template<typename T, typename ...Args>
void ECSManager::AddComponent(Entity entity, Args && ...args)
{
    size_t typeID = Component<T>::GetID();
    m_components[entity][typeID] = std::make_unique<T>(std::forward<Args>(args)...);
    m_entityComponentMasks[entity].set(typeID);

}

template<typename T>
T& ECSManager::GetComponent(Entity entity) {
    size_t typeID = Component<T>::GetID();
    return *static_cast<T*>(m_components[entity][typeID].get());
}

template<typename T>
bool ECSManager::HasComponent(Entity entity)
{
    size_t typeID = Component<T>::GetID();

    // Verificar si la entidad tiene el componente antes de acceder
    auto it = m_components[entity].find(typeID);
    return it != m_components[entity].end() && it->second != nullptr;
}


template<typename T>
void ECSManager::RemoveComponent(Entity entity) {
    size_t typeID = Component<T>::GetID();
    m_components[entity].erase(typeID);
    m_entityComponentMasks[entity].reset(typeID);
}

template<typename T, typename ...Args>
void ECSManager::RegisterSystem(Args && ...args)
{
    auto system = std::make_shared<T>(std::forward<Args>(args)...);
    m_systems.push_back(system);
}

template<typename... Components>
inline std::vector<Entity> ECSManager::GetEntitiesWithComponent()
{
	std::vector<Entity> entitiesWithComponent;

	for (auto entity : m_entities)
	{
		if ((HasComponent<Components>(entity) && ...))
		{
			entitiesWithComponent.push_back(entity);
		}
	}

	return entitiesWithComponent;
}
