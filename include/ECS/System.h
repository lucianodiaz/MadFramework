#pragma once
#include <functional>
#include <vector>
#include "Entity.h"

class ISystem {

public:
	virtual ~ISystem() = default;
	virtual void Update(float deltaTime) = 0;
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