#pragma once
#include <SFML/Graphics.hpp>


class ISceneTransition
{
public:
	ISceneTransition() {};

	virtual void OnStart() = 0;

	virtual void Update(float deltaTime) = 0;

	virtual void Draw(sf::RenderWindow& window) = 0;

	virtual bool IsFinished() const = 0;

	virtual void OnEnd() = 0;

	virtual ~ISceneTransition() {};

	sf::RectangleShape& GetOverlay() { return m_overlay; }

protected:
	sf::RectangleShape m_overlay;
};