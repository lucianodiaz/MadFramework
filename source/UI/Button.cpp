#include <UI/Button.h>

Button::FunctionType Button::DefaultOnClickFunction = []() {
	// Default action for button click
	};

Button::Button() : OnClick(DefaultOnClickFunction), m_state(None)
{
	SetPosition(0, 0);
	m_renderTexture.create(100, 50); // Default size, can be changed later
	m_shape.setSize(sf::Vector2f(100, 50)); // Default size, can be changed later
	//m_renderTexture.clear(sf::Color::Transparent);

	m_shape.setFillColor(m_normalColor);
	m_shape.setOutlineThickness(2.0f);
	m_shape.setOutlineColor(sf::Color::Black);

	m_renderTexture.draw(m_shape);
	m_renderTexture.display();
}

Button::~Button()
{
}

sf::Vector2f Button::GetSize() const
{
	sf::FloatRect bounds = m_sprite.getGlobalBounds();
	return sf::Vector2f(bounds.width, bounds.height);
}

void Button::Draw(sf::RenderWindow& window)
{
	if(!m_visible)
		return;
	sf::RenderStates states;
	sf::Vector2f position;
	if (m_parent)
	{
		position = m_parent->GetGlobalPosition() + m_position;
	}
	else
	{
		position = m_position;
	}
	if (!m_imageSetted)
	{

		//const sf::Texture& texture = m_renderTexture.getTexture();
		//
		////m_sprite.setTexture(texture);
	
	}

	m_sprite.setPosition(position);
	states.transform.translate(position);
	window.draw(m_sprite);
}

void Button::OnMouseEnter()
{
	const float light = 1.4f;
	m_sprite.setColor(m_hoveredColor);
}

void Button::OnMouseLeave()
{
	m_sprite.setColor(m_normalColor);
}

void Button::UpdateShape()
{
	if (m_fitToContent)
	{
		float maxWidth = 0;
		float maxHeight = 0;

		for(const auto& child : m_children)
		{
			sf::Vector2f childSize = child->GetSize();
			float widgetWidth = childSize.x;
			float widgetHeight = childSize.y;
			if (widgetHeight > maxHeight)
			{
				maxHeight = widgetHeight;
			}
			if (widgetWidth > maxWidth)
			{
				maxWidth = widgetWidth;
			}
		}
		m_renderTexture.create(static_cast<unsigned int>(maxWidth + m_margin), static_cast<unsigned int>(maxHeight + m_margin)); // Create render texture with the maximum width found
		m_shape.setSize(sf::Vector2f(maxWidth+m_margin, maxHeight + m_margin)); // Default height, can be changed later
		m_shape.setFillColor(m_normalColor);
		m_shape.setOutlineThickness(2.0f);
		m_shape.setOutlineColor(sf::Color::Black);

		m_renderTexture.draw(m_shape);
		m_renderTexture.display();
		const sf::Texture& texture = m_renderTexture.getTexture();

		m_sprite.setTextureRect(sf::IntRect(m_shape.getPosition().x,m_shape.getPosition().y,m_shape.getSize().x,m_shape.getSize().y));
		m_sprite.setTexture(texture);
	}

	if (m_parent)
	{
		Widget::UpdateShape();
	}
}

void Button::ProcessInput(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		const sf::Vector2f globalPosition = GetGlobalPosition();
		const sf::Vector2f size = GetSize();

		sf::FloatRect rect;
		rect.left = globalPosition.x;
		rect.top = globalPosition.y;
		rect.width = size.x;
		rect.height = size.y;

		if (rect.contains(event.mouseButton.x, event.mouseButton.y))
		{
			OnClick();
		}
	}
	else if(event.type == sf::Event::MouseMoved)
	{
		const sf::Vector2f globalPosition = GetGlobalPosition();
		const sf::Vector2f size = GetSize();

		sf::FloatRect rect;
		rect.left = globalPosition.x;
		rect.top = globalPosition.y;
		rect.width = size.x;
		rect.height = size.y;

		unsigned int oldState = m_state;
		m_state = None;

		const sf::Vector2f mousePosition(event.mouseMove.x, event.mouseMove.y);
		if (rect.contains(mousePosition))
		{
			m_state = Hovered;
		}
		if ((oldState & State::Hovered) && !(m_state & State::Hovered))
		{
			OnMouseLeave();
		}
		else if (!(oldState & State::Hovered) && (m_state & State::Hovered))
		{
			OnMouseEnter();
		}
	}
	

}
