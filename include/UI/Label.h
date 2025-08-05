#pragma once
#include "Widget.h"
#include <string>

class Label : public Widget
{
public:
	Label(const std::string& text);
	virtual ~Label();

	void SetText(const std::string& text);
	const std::string& GetText() const;

	void SetFillColor(const sf::Color& color);

	void SetOutlineColor(const sf::Color& color);

	void SetOutlineThickness(float thickness);

	void SetCharacterSize(unsigned int size);

	void SetFont(const sf::Font& font);

	sf::Vector2f GetSize() const override;

protected:
	virtual void Draw(sf::RenderWindow& window) override;

private:
	sf::Text m_text; // SFML text object for rendering the label
};