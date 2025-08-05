#include <UI/CanvasPanel.h>
#include <Core/World.h>

CanvasPanel::CanvasPanel()
{
	SetPosition(0.0f, 0.0f);
}

sf::Vector2f CanvasPanel::GetSize() const
{
	auto size = World::GetWorld()->GetWindow().GetRenderWindow().getSize();
	return sf::Vector2f(size.x, size.y);
}

void CanvasPanel::Draw(sf::RenderWindow& window)
{
	for (const auto child : m_children)
	{
		child->Draw(window);
	}
}

void CanvasPanel::UpdateShape()
{

	for(auto & child : m_children)
	{
		sf::Vector2f canvasSize = GetSize();
		sf::Vector2f childSize = child->GetSize();
		sf::Vector2f childPosition;

		switch (child->GetAnchor())
		{
		case Anchor::TopLeft:
			childPosition = sf::Vector2f(0, 0);
			break;
		case Anchor::TopCenter:
			childPosition = sf::Vector2f((canvasSize.x - childSize.x) / 2.0f, 0);
			break;
		case Anchor::TopRight:
			childPosition = sf::Vector2f(canvasSize.x - childSize.x, 0);
			break;
		case Anchor::CenterLeft:
			childPosition = sf::Vector2f(0, (canvasSize.y - childSize.y) / 2.0f);
			break;
		case Anchor::Center:
			childPosition = sf::Vector2f((canvasSize.x - childSize.x) / 2.0f, (canvasSize.y - childSize.y) / 2.0f);
			break;
		case Anchor::CenterRight:
			childPosition = sf::Vector2f(canvasSize.x - childSize.x, (canvasSize.y - childSize.y) / 2.0f);
			break;
		case Anchor::BottomLeft:
			childPosition = sf::Vector2f(0, canvasSize.y - childSize.y);
			break;
		case Anchor::BottomCenter:
			childPosition = sf::Vector2f((canvasSize.x - childSize.x) / 2.0f, canvasSize.y - childSize.y);
			break;
		case Anchor::BottomRight:
			childPosition = sf::Vector2f(canvasSize.x - childSize.x, canvasSize.y - childSize.y);
			break;
		default:
			break;

		}

		child->SetPosition(childPosition);
	}

}
