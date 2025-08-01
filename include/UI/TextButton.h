#pragma once
#include "Button.h"
#include "Label.h"

class TextButton : public Button
{
public:
	TextButton(const std::string text, Widget* parent = nullptr);
	TextButton(const std::string text, Widget* parent, FuncType OnClick);

	virtual ~TextButton();
	void SetText(const std::string& text);
	void SetCharacterSize(unsigned int size);
	void SetTextColor(const sf::Color& color);
	void SetFillColor(const sf::Color& color);
	void SetOutlineColor(const sf::Color& color);
	void SetOutlineThickness(float thickness);
	virtual sf::Vector2f GetSize() const override;
	const std::string& GetText() const;

private:
	sf::RectangleShape m_shape;
	Label m_label;
	sf::Color m_fillColor;
	sf::Color m_outlineColor;

	void UpdateShape() override;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	virtual void onMouseEntered() override;
	virtual void onMouseLeft() override;
};

