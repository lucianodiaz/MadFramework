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

	virtual void SetParent(Widget* parent);

protected:
	friend class Container;
	friend class VerticalLayout;
	//friend class HorizontalLayout;
	//frind class GridLayout;

	virtual bool ProcessEvent(const sf::Event& event, const sf::Vector2f& parent_position);
	virtual void ProcessEvents(const sf::Vector2f& parent_position);
	virtual void UpdateShape();


	Widget* m_parent;
	sf::Vector2f m_position;
};