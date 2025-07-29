#pragma once
#include <SFML/Graphics.hpp>


class Widget : public sf::Drawable
{
public:
	Widget(Widget* parent = nullptr);
	virtual ~Widget();

	void SetPosition(float x, float y);
	void SetPosition(const sf::Vector2f& position);

	const sf::Vector2f& GetPosition() const;

	virtual sf::Vector2f GetSize() const = 0;

	virtual Widget* GetParent() const;

protected:
	virtual bool processEvent(const sf::Event& event, const sf::Vector2f& parent_position);
	virtual void processEvents(const sf::Vector2f& parent_position);
	virtual void updateShape();


	Widget* m_parent;
	sf::Vector2f m_position;
};