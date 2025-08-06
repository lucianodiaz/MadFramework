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
	for (auto& child : m_children)
	{

		if (child->GetParent() == shared_from_this())
		{
			sf::Vector2f canvasSize = GetSize();
			sf::Vector2f childSize = child->GetSize();
			sf::Vector2f anchorBase;

			switch (child->GetAnchor())
			{
			case Anchor::TopLeft:
				anchorBase = sf::Vector2f(0, 0);
				break;
			case Anchor::TopCenter:
				anchorBase = sf::Vector2f((canvasSize.x - childSize.x) / 2.0f, 0);
				break;
			case Anchor::TopRight:
				anchorBase = sf::Vector2f(canvasSize.x - childSize.x, 0);
				break;
			case Anchor::CenterLeft:
				anchorBase = sf::Vector2f(0, (canvasSize.y - childSize.y) / 2.0f);
				break;
			case Anchor::Center:
				anchorBase = sf::Vector2f((canvasSize.x - childSize.x) / 2.0f, (canvasSize.y - childSize.y) / 2.0f);
				break;
			case Anchor::CenterRight:
				anchorBase = sf::Vector2f(canvasSize.x - childSize.x, (canvasSize.y - childSize.y) / 2.0f);
				break;
			case Anchor::BottomLeft:
				anchorBase = sf::Vector2f(0, canvasSize.y - childSize.y);
				break;
			case Anchor::BottomCenter:
				anchorBase = sf::Vector2f((canvasSize.x - childSize.x) / 2.0f, canvasSize.y - childSize.y);
				break;
			case Anchor::BottomRight:
				anchorBase = sf::Vector2f(canvasSize.x - childSize.x, canvasSize.y - childSize.y);
				break;
			}

			//
			child->m_computedPosition = anchorBase + child->GetOffsetFromAnchor();
		}
		else
		{
			child->m_computedPosition = child->GetOffsetFromAnchor();
		}
		
		
	}
}
