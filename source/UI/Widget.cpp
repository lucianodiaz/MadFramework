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
	RequestLayout();
}

void Widget::SetPosition(sf::Vector2f position)
{
	m_offsetFromAnchor = position;
	RequestLayout();
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
		RequestLayout();
	}

}

void Widget::SetAnchor(Anchor anchor)
{
	m_anchor = anchor;
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
	/*if (m_parent)
	{
		m_parent->UpdateShape();
	}*/
}

void Widget::RequestLayout()
{
	auto root = GetRoot();
	if (root->m_layoutInProgress) return;
	root->m_layoutInProgress = true;
	root->UpdateShape();
	root->m_layoutInProgress = false;
}
