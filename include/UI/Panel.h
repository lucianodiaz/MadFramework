#pragma once
#include "Widget.h"


class Panel : public Widget
{
public:
	Panel() {};
	virtual ~Panel() = default;

	virtual sf::Vector2f GetSize() const override;

	void SetBackgroundColor(const sf::Color& color) { m_backgroundColor = color; }

protected:
	void Draw(sf::RenderWindow& window) override {};
	void UpdateShape() override {};


	sf::Color m_backgroundColor = sf::Color::Transparent; // Default background color
};