#pragma once
#include <unordered_map>
#include <MadFrameworkExport.h>
#include "Action.h"

template<typename T = int>
class MAD_API ActionMap
{
public:
	ActionMap(const ActionMap<T>&) = delete;
	ActionMap<T>& operator=(const ActionMap<T>&) = delete;

	ActionMap() = default;

	void map(const T& key, const Action& action);

	const Action& get(const T& key)const;

private:
	std::unordered_map<T, Action> _map;

};

template<typename T>
inline void ActionMap<T>::map(const T& key, const Action& action)
{
	_map.emplace(key, action);
}

template<typename T>
inline const Action& ActionMap<T>::get(const T& key) const
{
	// // O: insert return statement here
	return _map.at(key);
}
