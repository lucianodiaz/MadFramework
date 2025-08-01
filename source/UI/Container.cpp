#include "UI/Container.h"

Container::Container(Widget* parent) : Widget(parent), m_layout(nullptr)
{
}

Container::~Container()
{
	if (m_layout != nullptr && m_layout->m_parent == this)
	{
		m_layout->m_parent = nullptr;
		delete m_layout;
	}
}

void Container::SetLayout(Layout* layout)
{
	if (m_layout != nullptr && m_layout->m_parent == this)
	{
		m_layout->m_parent = nullptr;
	}
	if ((m_layout = layout) != nullptr)
	{
		m_layout->m_parent = this;
		m_layout->UpdateShape();
	}
}

Layout* Container::GetLayout() const
{
	return m_layout;
}

sf::Vector2f Container::GetSize() const
{
	sf::Vector2f res(0, 0);

	if (m_layout)
	{
		res = m_layout->GetSize();
	}

	return res;
}

void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_layout)
	{
		target.draw(*m_layout, states);
	}
}

bool Container::ProcessEvent(const sf::Event& event, const sf::Vector2f& parent_position)
{
	bool result = false;
	if (m_layout)
	{
		result = m_layout->ProcessEvent(event, parent_position);
	}
	return result;
}

void Container::ProcessEvents(const sf::Vector2f& parent_position)
{
	if (m_layout)
	{
		m_layout->ProcessEvents(parent_position);
	}
}
