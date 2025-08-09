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
	float totalHeight = 0.0f;
	const size_t n = m_children.size();

	for (const auto& child : m_children)
	{
		const sf::Vector2f size = child->GetSize();
		if (size.x > maxWidth) maxWidth = size.x;
		totalHeight += size.y;
	}

	// spacing entre elementos: (n - 1) gaps
	if (n > 1) totalHeight += m_spacing * static_cast<float>(n - 1);

	// si querés padding externo simétrico:
	const float pad = m_spacing;
	return sf::Vector2f(maxWidth + 2.0f * pad, totalHeight + 2.0f * pad);
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
	if (m_children.empty()) return;

	float maxWidth = 0.0f;
	for (const auto& child : m_children)
		maxWidth = std::max(maxWidth, child->GetSize().x);

	const float pad = m_spacing;

	float y = pad; // padding superior
	for (const auto& child : m_children)
	{
		const sf::Vector2f size = child->GetSize();
		const float x = pad + (maxWidth - size.x) * 0.5f; // centrado horizontal
		child->SetLayoutPosition({ x, y });
		child->UpdateShape();
		y += size.y + m_spacing; // spacing entre filas
	}
}
