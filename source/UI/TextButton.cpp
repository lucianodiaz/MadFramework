#include "UI/TextButton.h"
#include "UI/Label.h"

TextButton::TextButton(const std::string text, Widget* parent)
	: Button(parent), m_label(text, this)
{
	SetOutlineThickness(5.0f);
	SetFillColor(sf::Color::White);
	SetOutlineColor(sf::Color::Black);
}

TextButton::TextButton(const std::string text, Widget* parent, FuncType OnClick) 
	: Button(parent, OnClick), m_label(text, this)
{
	SetOutlineThickness(5.0f);
	SetFillColor(sf::Color::White);
	SetOutlineColor(sf::Color::Black);
}
TextButton::~TextButton()
{
}

void TextButton::SetText(const std::string& text)
{
	m_label.SetText(text);
	//UpdateShape();
}

void TextButton::SetCharacterSize(unsigned int size)
{
	m_label.SetCharacterSize(size);
	//UpdateShape();
}

void TextButton::SetTextColor(const sf::Color& color)
{
	m_label.SetColor(color);
}

void TextButton::SetFillColor(const sf::Color& color)
{
	m_fillColor = color;
	m_shape.setFillColor(color);
}

void TextButton::SetOutlineColor(const sf::Color& color)
{
	m_outlineColor = color;
	m_shape.setOutlineColor(color);
}

void TextButton::SetOutlineThickness(float thickness)
{
	m_shape.setOutlineThickness(thickness);
	//UpdateShape();
}

sf::Vector2f TextButton::GetSize() const
{
	sf::FloatRect rect = m_shape.getGlobalBounds();
	return sf::Vector2f(rect.width, rect.height);
}

void TextButton::UpdateShape()
{
	sf::Vector2f labelSize = m_label.GetSize();
	unsigned int charSize = m_label.GetCharacterSize();
	m_shape.setSize(sf::Vector2f(charSize * 2.0f + labelSize.x, charSize + labelSize.y));
	m_label.SetPosition(charSize, charSize);
	Widget::UpdateShape();
}

void TextButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform.translate(m_position);
	target.draw(m_shape, states);
	target.draw(m_label, states);
}

void TextButton::onMouseEntered()
{
	const float light = 1.4f;
	m_shape.setOutlineColor(sf::Color(
		static_cast<sf::Uint8>(m_outlineColor.r * light),
		static_cast<sf::Uint8>(m_outlineColor.g * light),
		static_cast<sf::Uint8>(m_outlineColor.b * light)
	));
	m_shape.setFillColor(sf::Color(
		static_cast<sf::Uint8>(m_fillColor.r * light),
		static_cast<sf::Uint8>(m_fillColor.g * light),
		static_cast<sf::Uint8>(m_fillColor.b * light)
	));
}

void TextButton::onMouseLeft()
{
	m_shape.setOutlineColor(m_outlineColor);
	m_shape.setFillColor(m_fillColor);
}
