#pragma once

template<typename T, typename ...Args>
T& ECSManager::AddComponent(Entity entity, Args && ...args)
{
	std::string autoName = std::string(typeid(T).name()) + "#" + std::to_string(m_componentsMulti[entity][Component<T>::GetID()].size() + 1);

	const size_t typeID = Component<T>::GetID();
	auto& vec = m_componentsMulti[entity][typeID];
	auto& byName = m_nameIndex[entity][typeID];

	std::string key(autoName);
	if (key.empty()) key = "unnamed";
	if (byName.count(key))
	{
		int n = 2;
		std::string base = std::string(autoName);
		do
		{
			key = base + "_" + std::to_string(n++);
		} while (byName.count(key));

	}

	ComponentEntry entry;
	entry.name = key;
	entry.ptr = std::make_unique<T>(std::forward<Args>(args)...);
	entry.instanceID = static_cast<uint32_t>(vec.size());
	vec.emplace_back(std::move(entry));
	byName[key] = vec.size() - 1;
	m_entityComponentMasks[entity].set(typeID);

	return *static_cast<T*>(vec.back().ptr.get());

}

template<typename T, typename ...Args>
inline T& ECSManager::AddComponent(Entity entity, std::string_view name, Args && ...args)
{
	const size_t typeID = Component<T>::GetID();
	auto& vec = m_componentsMulti[entity][typeID];
	auto& byName = m_nameIndex[entity][typeID];

	std::string key(name);
	if (key.empty()) key = "unnamed";
	if (byName.count(key))
	{
		int n = 2;
		std::string base = std::string(name);
		do
		{
			key = base + "_" + std::to_string(n++);
		} while (byName.count(key));

	}

	ComponentEntry entry;
	entry.name = key;
	entry.ptr = std::make_unique<T>(std::forward<Args>(args)...);
	entry.instanceID = static_cast<uint32_t>(vec.size());
	vec.emplace_back(std::move(entry));
	byName[key] = vec.size() - 1;
	m_entityComponentMasks[entity].set(typeID);

	return *static_cast<T*>(vec.back().ptr.get());
}

template<typename T>
inline T& ECSManager::GetComponent(Entity entity)
{
	return *static_cast<T*>(GetComponents<T>(entity)[0]);
}

template<typename T>
inline std::vector<T*> ECSManager::GetComponents(Entity entity)
{
	std::vector<T*> out;

	const size_t typeID = Component<T>::GetID();
	auto eIt = m_componentsMulti.find(entity);
	if (eIt == m_componentsMulti.end()) return out;//return an empty vector

	auto tIt = eIt->second.find(typeID);
	if (tIt == eIt->second.end()) return out;//return an empty vector

	out.reserve(tIt->second.size());

	for (auto& entry : tIt->second)
	{
		out.push_back(static_cast<T*>(entry.ptr.get()));
	}

	return out;

}

template<typename T>
inline T& ECSManager::GetComponentByName(Entity entity, std::string_view name)
{
	const size_t typeID = Component<T>::GetID();

	auto eIt = m_componentsMulti.find(entity);
	if (eIt == m_componentsMulti.end()) throw std::out_of_range("Entity not found");

	auto tIt = eIt->second.find(typeID);
	if (tIt == eIt->second.end()) throw std::out_of_range("Type not found");

	auto nIt = m_nameIndex[entity][typeID].find(std::string(name));
	if (nIt == m_nameIndex[entity][typeID].end()) throw std::out_of_range("Name not found");

	size_t idx = nIt->second;
	return *static_cast<T*>(tIt->second.at(idx).ptr.get());

}

template<typename T>
inline bool ECSManager::HasComponent(Entity entity) const
{
	const size_t typeID = Component<T>::GetID();
	auto eIt = m_componentsMulti.find(entity);
	if (eIt == m_componentsMulti.end()) return false;
	auto tIt = eIt->second.find(typeID);
	return (tIt != eIt->second.end()) && !tIt->second.empty();
}

template<typename T>
inline bool ECSManager::RemoveComponent(Entity entity, size_t index)
{
	const size_t typeID = Component<T>::GetID();

	auto& vec = m_componentsMulti[entity][typeID];
	if (index >= vec.size()) return false;

	auto& names = m_nameIndex[entity][typeID];
	std::string removedName = vec[index].name;

	if (index != vec.size() - 1)
	{
		std::swap(vec[index], vec.back());
		names[vec[index].name] = index;
	}

	vec.pop_back();
	names.erase(removedName);

	if (vec.empty()) m_entityComponentMasks[entity].reset(typeID);

	return true;
}

template<typename T>
inline bool ECSManager::RemoveComponentByName(Entity entity, std::string_view name)
{
	const size_t typeID = Component<T>::GetID();
	auto& names = m_nameIndex[entity][typeID];
	auto it = names.find(std::string(name));
	if (it == names.end()) return false;
	return RemoveComponent<T>(entity, it->second);
}

template<typename T>
inline bool ECSManager::RenameComponent(Entity e, std::string_view oldName, std::string_view newName)
{
	const size_t typeID = Component<T>::GetID();

	auto& names = m_nameIndex[e][typeID];

	auto it = names.find(std::string(oldName));
	if (it == names.end()) return false;

	if (names.count(std::string(newName))) return false;

	size_t idx = it->second;

	names.erase(it);
	names[std::string(newName)] = idx;
	m_componentsMulti[e][typeID][idx].name = std::string(newName);

	return true;
}

template<typename T, typename ...Args>
void ECSManager::RegisterSystem(Args && ...args)
{
	auto system = std::make_shared<T>(std::forward<Args>(args)...);
	m_systems.emplace_back(system);
}

template<typename... Components>
inline std::vector<Entity> ECSManager::GetEntitiesWithComponent()
{
	std::vector<Entity> entitiesWithComponent;

	for (auto entity : m_entities)
	{
		if ((HasComponent<Components>(entity) && ...))
		{
			entitiesWithComponent.emplace_back(entity);
		}
	}

	return entitiesWithComponent;
}
