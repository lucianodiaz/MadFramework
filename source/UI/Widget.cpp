#include <UI/Widget.h>

Widget::Widget() : m_visible(true)
{
	m_computedPosition = sf::Vector2f(0.0f, 0.0f);
	m_offsetFromAnchor = sf::Vector2f(0.0f, 0.0f);
	m_children.clear();
}

Widget::~Widget()
{
}

void Widget::SetPosition(float x, float y)
{
	m_offsetFromAnchor = sf::Vector2f(x, y);
	UpdateShape();
}

void Widget::SetPosition(sf::Vector2f position)
{
	m_offsetFromAnchor = position;
	UpdateShape();
}

void Widget::SetParent(std::shared_ptr<Widget> parent)
{
	m_parent = parent;
}

void Widget::AddChild(std::shared_ptr<Widget> child)
{
	if (child != m_parent)
	{
		m_children.push_back(child);
		child->SetParent(shared_from_this());
		UpdateShape();
	}

}

void Widget::SetAnchor(Anchor anchor)
{
	m_anchor = anchor;
	UpdateShape();
}

sf::Vector2f Widget::GetGlobalPosition() const
{
	if (m_parent)
	{
		return m_parent->GetGlobalPosition() + m_computedPosition;
	}
	return m_computedPosition;
}

void Widget::Hide()
{
	m_visible = false;
}

void Widget::Show()
{
	m_visible = true;
}

bool Widget::IsVisible() const
{
	return m_visible;
}

void Widget::UpdateShape()
{
	if (m_parent)
	{
		m_parent->UpdateShape();
	}
}
