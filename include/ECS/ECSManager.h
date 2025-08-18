#pragma once
#include "Component.h"
#include "Entity.h"
#include "System.h"
#include <bitset>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>


namespace sf { class RenderWindow; }

constexpr size_t MAX_COMPONENTS = 32;

struct ComponentEntry
{
	std::string name;
	std::unique_ptr<IComponent> ptr;
	uint32_t instanceID;
};

class ECSManager
{
public:
	ECSManager() = default;

	Entity CreateEntity();


	void DestroyEntity(Entity entity);

	template<typename T, typename... Args>
	T& AddComponent(Entity entity, Args&&... args);

	template<typename T, typename... Args>
	T& AddComponent(Entity entity, std::string_view name, Args&&... args);

	template<typename T>
	T& GetComponent(Entity entity);

	template<typename T>
	std::vector<T*> GetComponents(Entity entity);

	template<typename T>
	T& GetComponentByName(Entity entity, std::string_view name);

	template<typename T>
	bool HasComponent(Entity entity) const;

	template<typename T>
	bool RemoveComponent(Entity entity, size_t index);

	template<typename T>
	bool RemoveComponentByName(Entity entity, std::string_view name);

	template<typename T>
	bool RenameComponent(Entity e, std::string_view oldName, std::string_view newName);

	template<typename T, typename... Args>
	void RegisterSystem(Args&&... args);

	void Update(float deltaTime);

	void Draw(sf::RenderWindow& window);

	std::vector<std::shared_ptr<ISystem>>& GetSystems() { return m_systems; }

	std::vector<Entity> GetEntities();

	template<typename... Components>
	std::vector<Entity> GetEntitiesWithComponent();


private:

	std::vector<Entity> m_entities;
	std::unordered_map<Entity, std::bitset<MAX_COMPONENTS>> m_entityComponentMasks;
	std::vector<std::shared_ptr<ISystem>> m_systems;

	std::unordered_map < Entity, std::unordered_map<size_t, std::vector<ComponentEntry>>> m_componentsMulti;
	std::unordered_map<Entity, std::unordered_map<size_t, std::unordered_map<std::string, size_t>>> m_nameIndex;
};

#include "ECSManager.inl"