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
}

void HorizontalLayout::UpdateShape()
{
	float maxHeight = 0.0f;
	if (m_children.empty()) return;
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
		sf::Vector2f parentSize = GetSize();

		//Ahora estamos centrando todo al medio, lo que queremos es que se ubiquen dependiendo de su alignment
		//child->SetLayoutPosition({ x, (maxHeight - size.y) / 2.0f }); // Centrado vertical

		switch (child->GetVerticalAlignment())
		{

		case VerticalAlignment::Top:
			child->SetLayoutPosition(sf::Vector2f(x, m_spacing));
			break;
		case VerticalAlignment::Center:
			child->SetLayoutPosition(sf::Vector2f(x, (parentSize.y - size.y) / 2.0f));
			break;
		case VerticalAlignment::Bottom:
			child->SetLayoutPosition(sf::Vector2f(x, (parentSize.y - size.y)));
			break;

		}

		x += size.x;
		if (i < m_children.size() - 1) {
			x += m_spacing; 
		}
	}
}
