#include <UI/Widget.h>


Widget::Widget(Widget* parent) : m_parent(parent), m_position(0.f, 0.f)
{
}

Widget::~Widget()
{

}

void Widget::SetPosition(float x, float y)
{
	m_position.x = x;
	m_position.y = y;
}

void Widget::SetPosition(const sf::Vector2f& position)
{
	m_position = position;
}

const sf::Vector2f& Widget::GetPosition() const
{
	return m_position;
}

bool Widget::ProcessEvent(const sf::Event& event, const sf::Vector2f& parent_position)
{
	return false;
}
void Widget::ProcessEvents(const sf::Vector2f& parent_position)
{
}

Widget* Widget::GetParent() const
{
	return m_parent;
}

void Widget::SetParent(Widget* parent)
{
	m_parent = parent;
}

void Widget::UpdateShape()
{
	if (m_parent)
	{
		m_parent->UpdateShape();
	}
}