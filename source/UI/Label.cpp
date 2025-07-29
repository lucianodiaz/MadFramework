#include <UI/Label.h>
#include <Core/World.h>
#include <Core/Signal.h>

Label::Label(const std::string& text, Widget* parent)
	: Widget(parent)
{
	SetText(text);
	sf::Font defaultFont = World::GetWorld()->GetFont("defaultFont");
	SetFont(defaultFont);
	SetColor(sf::Color::White);
}

Label::Label(const std::string& text, const sf::Font& font, Widget* parent)
	: Widget(parent)
{
	SetText(text);
	SetFont(font);
	SetColor(sf::Color::White);
}

Label::~Label()
{
}

void Label::SetText(const std::string& text)
{
	m_text.setString(text);
	UpdateShape();
}

void Label::SetFont(const sf::Font& font)
{
	m_text.setFont(font);
	UpdateShape();
}

void Label::SetCharacterSize(unsigned int size)
{
	m_text.setCharacterSize(size);
	UpdateShape();
}

void Label::SetColor(const sf::Color& color)
{
	m_text.setFillColor(color);
	UpdateShape();
}

const std::string& Label::GetText() const
{
	return m_text.getString();
}

sf::Vector2f Label::GetSize() const
{
	sf::FloatRect rect = m_text.getLocalBounds();


	return sf::Vector2f(rect.width, rect.height);
}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform.translate(m_position);
	target.draw(m_text, states);
}
