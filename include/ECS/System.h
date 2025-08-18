#pragma once
#include <functional>
#include <vector>
#include "Entity.h"
#include <SFML/Graphics.hpp>

class ISystem {

public:
	virtual ~ISystem() = default;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(sf::RenderWindow&) {}
};

template<typename...Components>
class System : public ISystem
{
public:
	void Update(float deltaTime) override
	{
		UpdateEntities(deltaTime);
	}

protected:

	virtual void UpdateEntities(float deltaTime)=0;
};