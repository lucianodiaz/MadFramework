#include <UI/Panel.h>

sf::Vector2f Panel::GetSize() const
{
	float width = 0.0f;
	float height = 0.0f;

	for (const auto& child : m_children)
	{
		sf::Vector2f childSize = child->GetSize();
		
		width += childSize.x;
		height += childSize.y;
	}

    return sf::Vector2f(width, height);
}
