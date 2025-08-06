#pragma once
#include "Widget.h"

class Image : public Widget
{
public:

	Image();
	virtual ~Image();
	virtual sf::Vector2f GetSize() const override;
	void SetImage(const sf::Texture& texture);

protected:
	void Draw(sf::RenderWindow& window) override;

private:
	sf::Sprite m_sprite;
	sf::Texture m_texture;
};