#pragma once
#include <SFML/Graphics.hpp>
#include <MadFrameworkExport.h>

class MAD_API ISceneTransition
{
public:
	ISceneTransition() {};

	virtual void OnStart() = 0;

	virtual void Update(float deltaTime) = 0;

	virtual bool IsFinished() const = 0;

	virtual void OnEnd() = 0;

	virtual ~ISceneTransition() {};

	sf::RectangleShape& GetOverlay() { return m_overlay; }

protected:
	sf::RectangleShape m_overlay;
};