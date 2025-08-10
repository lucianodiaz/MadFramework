#include <UI/Label.h>
#include <Core/World.h>

Label::Label(const std::string& text)
{
	//SetPosition(0, 0);
	SetText(text);

	SetFont(World::GetWorld()->GetFont("defaultFont"));
}

Label::~Label()
{
}

void Label::SetText(const std::string& text)
{
	m_text.setString(text);
	RequestLayout();
}

const std::string& Label::GetText() const
{
	return m_text.getString();
}

void Label::SetFillColor(const sf::Color& color)
{
	m_text.setFillColor(color);
	
}

void Label::SetOutlineColor(const sf::Color& color)
{
	m_text.setOutlineColor(color);
}

void Label::SetOutlineThickness(float thickness)
{
	m_text.setOutlineThickness(thickness);
	
}

void Label::SetCharacterSize(unsigned int size)
{
	m_text.setCharacterSize(size);
	RequestLayout();
}

void Label::SetFont(const sf::Font& font)
{
	m_text.setFont(font);
	RequestLayout();
}

sf::Vector2f Label::GetSize() const
{
	sf::FloatRect rect = m_text.getLocalBounds();

	return sf::Vector2f(rect.width, rect.height);
}

void Label::Draw(sf::RenderWindow& window)
{
	if (m_parent && !m_parent->IsVisible()) return;
	if (!m_visible)
		return;
	sf::RenderStates states;
	sf::Vector2f position;
	if(m_parent)
	{
		position = m_parent->GetGlobalPosition() + m_computedPosition;
	}
	else
	{
		position = m_computedPosition;
	}
	states.transform.translate(position);
	window.draw(m_text, states);
}
