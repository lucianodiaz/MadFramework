#include <UI/VerticalLayout.h>

VerticalLayout::VerticalLayout() : m_spacing(5.0f)
{
}

VerticalLayout::~VerticalLayout()
{
}

sf::Vector2f VerticalLayout::GetSize() const
{
	float maxWidth = 0.0f;
	float y = 0.0f;

	for (const auto& child : m_children)
	{
		sf::Vector2f size = child->GetSize();
		if(size.x > maxWidth)
		{
			maxWidth = size.x + m_spacing;

			y += size.y + m_spacing;
		}
	}


	return sf::Vector2f(maxWidth * 2.0f, y + m_spacing);
}

void VerticalLayout::Draw(sf::RenderWindow& window)
{
	for (const auto child : m_children)
	{
		child->Draw(window);
	}
}

void VerticalLayout::UpdateShape()
{

	float max_x = 0.0f;

	for (const auto& child : m_children)
	{
		sf::Vector2f size = child->GetSize();

		if(size.x > max_x)
		{
			max_x = size.x + m_spacing;
		}
	}

	float y = m_spacing;
	for (const auto& child : m_children)
	{
		sf::Vector2f size = child->GetSize();
		child->SetPosition(((max_x- size.x)/2.0f) + m_spacing, y);
		y += size.y + m_spacing;
	}

	Widget::UpdateShape();
}
