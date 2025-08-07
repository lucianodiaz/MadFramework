#include <UI/HorizontalLayout.h>

HorizontalLayout::HorizontalLayout() : m_spacing(5.0f)
{
}

HorizontalLayout::~HorizontalLayout()
{
}

sf::Vector2f HorizontalLayout::GetSize() const
{
	float totalWidth = 0.0f;
	float maxHeight = 0.0f;

	for (const auto& child : m_children)
	{
		sf::Vector2f size = child->GetSize();
		totalWidth += size.x;
		if (size.y > maxHeight)
			maxHeight = size.y;
	}

	// Agregamos spacing entre los elementos (cantidad de espacios = n - 1)
	if (m_children.size() > 1)
	{
		totalWidth += m_spacing * (m_children.size() - 1);
	}

	return sf::Vector2f(totalWidth, maxHeight);
}

void HorizontalLayout::Draw(sf::RenderWindow& window)
{
	for (const auto child : m_children)
	{
		child->Draw(window);
	}
}

void HorizontalLayout::UpdateShape()
{
	float maxHeight = 0.0f;

	for (const auto& child : m_children)
	{
		sf::Vector2f size = child->GetSize();
		if (size.y > maxHeight)
		{
			maxHeight = size.y;
		}
	}

	float x = 0.0f;
	for (size_t i = 0; i < m_children.size(); ++i)
	{
		auto& child = m_children[i];
		sf::Vector2f size = child->GetSize();

		child->SetPosition(x, (maxHeight - size.y) / 2.0f); // Centrado vertical

		x += size.x;
		if (i < m_children.size() - 1) {
			x += m_spacing; 
		}
	}
}
