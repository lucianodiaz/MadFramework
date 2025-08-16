#include <UI/Widget.h>

Widget::Widget() : m_visible(true), m_root(nullptr)
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
	RequestLayout();
}

void Widget::SetPosition(sf::Vector2f position)
{
	m_offsetFromAnchor = position;
	RequestLayout();
}

void Widget::SetParent(std::shared_ptr<Widget> parent)
{
	if(parent)m_parent = parent;
	
}

void Widget::AddChild(std::shared_ptr<Widget> child)
{
	if (child != m_parent)
	{
		m_children.push_back(child);
		child->SetParent(shared_from_this());
		child->UpdateShape();
		RequestLayout();
	}

}

void Widget::SetAnchor(Anchor anchor)
{
	m_anchor = anchor;
	RequestLayout();
}

void Widget::SetVerticalAlignment(VerticalAlignment verticalAlignment)
{
	m_vAlignment = verticalAlignment;
	RequestLayout();
}

void Widget::SetHorizontalAlignment(HorizontalAlignment horizontalAlignment)
{
	m_hAlignment = horizontalAlignment;
	RequestLayout();
}

sf::Vector2f Widget::GetGlobalPosition() const
{
	if (m_parent)
	{
		return m_parent->GetGlobalPosition() + m_computedPosition;
	}
	return m_computedPosition;
}

Widget* Widget::GetRoot()
{
	// TODO: Insertar una instrucción "return" aquí
	Widget* w = this;
	while (w->m_parent) w = w->m_parent.get();
	return w;
}

void Widget::Hide()
{
	m_visible = false;
	for (auto c : m_children)
	{
		c->Hide();
	}
}

void Widget::Show()
{
	m_visible = true;
	for (auto c : m_children)
	{
		c->Show();
	}
}

bool Widget::IsVisible() const
{
	return m_visible;
}

void Widget::UpdateShape()
{
	if (m_parent)
	{
		sf::Vector2f parentSize = m_parent->GetSize();
		sf::Vector2f childSize = GetSize();
		sf::Vector2f anchorBase;
		float m_internalSpacing = 10.0f;
		switch (m_vAlignment)
		{
		case VerticalAlignment::Top:
			anchorBase = sf::Vector2f(0.0f, 0.0f);
			break;
		case VerticalAlignment::Center:
			anchorBase = sf::Vector2f(0.0f, ((parentSize.y - childSize.y) - m_internalSpacing) / 2.0f);
			break;
		case VerticalAlignment::Bottom:
			anchorBase = sf::Vector2f(0.0f, (parentSize.y - childSize.y) - m_internalSpacing);
			break;
		}

		switch (m_hAlignment)
		{
		case HorizontalAlignment::Left:
			anchorBase = anchorBase + sf::Vector2f(0.0f,0.0f);
			break;
		case HorizontalAlignment::Center:
			anchorBase = anchorBase + sf::Vector2f((parentSize.x - childSize.x) / 2.0f, 0.0f);
			break;
		case HorizontalAlignment::Right:
			anchorBase = anchorBase + sf::Vector2f((parentSize.x - childSize.x), 0.0f);
			break;
		}

		SetLayoutPosition(anchorBase + m_computedPosition);
	}



	
}

void Widget::RequestLayout()
{
	auto root = GetRoot();
	if (root->m_layoutInProgress) return;
	root->m_layoutInProgress = true;
	root->UpdateShape();
	root->m_layoutInProgress = false;
}
