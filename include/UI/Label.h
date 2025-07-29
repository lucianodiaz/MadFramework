#pragma once
#include <UI/Widget.h>
#include <string>

class Label : public Widget
{
public:
	Label(const std::string& text, Widget* parent = nullptr);
	Label(const std::string& text, const sf::Font& font, Widget* parent = nullptr);

	virtual ~Label();

	void SetText(const std::string& text);
	void SetFont(const sf::Font& font);
	void SetCharacterSize(unsigned int size);
	void SetColor(const sf::Color& color);

	const std::string& GetText() const;

	virtual sf::Vector2f GetSize() const override;

private:
	sf::Text m_text;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

};

