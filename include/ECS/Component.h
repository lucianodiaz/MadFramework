#pragma once
#include <unordered_map>
#include <memory>
#include "Entity.h"

class IComponent 
{
public:
	virtual ~IComponent()=default;
};

template <typename T>
class Component : public IComponent
{
public:
	static std::unordered_map<Entity, T> data;


	static size_t GetID()
	{
		static size_t id = GenerateID();
		return id;
	}

private:
	static size_t GenerateID()
	{
		static size_t lastID = 0;
		return lastID++;
	}
};

template <typename T>
std::unordered_map<Entity, T> Component<T>::data;

