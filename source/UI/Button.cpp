#include <UI/Button.h>

Button::FunctionType Button::DefaultOnClickFunction = [](Button&) {
	// Default action for button click
	};

Button::Button() : OnClick(DefaultOnClickFunction), m_state(None)
{
	//SetPosition(0, 0);
	m_renderTexture.create(m_sizeButton.x, m_sizeButton.y); // Default size, can be changed later
	m_shape.setSize(sf::Vector2f(m_sizeButton.x, m_sizeButton.y)); // Default size, can be changed later
	m_shape.setFillColor(m_normalColor);
	m_shape.setOutlineThickness(2.0f);
	m_shape.setOutlineColor(sf::Color::Black);

	m_renderTexture.draw(m_shape);
	m_renderTexture.display();
	const sf::Texture& texture = m_renderTexture.getTexture();

	m_sprite.setTextureRect(sf::IntRect(m_shape.getPosition().x, m_shape.getPosition().y, m_shape.getSize().x, m_shape.getSize().y));
	m_sprite.setTexture(texture);
}

Button::~Button()
{
}

void Button::SetButtonSize(sf::Vector2f newSize)
{
	m_sizeButton = newSize;
	m_renderTexture.create(m_sizeButton.x, m_sizeButton.y); // Default size, can be changed later
	m_shape.setSize(sf::Vector2f(m_sizeButton.x, m_sizeButton.y)); // Default size, can be changed later
	m_shape.setFillColor(m_normalColor);
	m_shape.setOutlineThickness(2.0f);
	m_shape.setOutlineColor(sf::Color::Black);

	m_renderTexture.draw(m_shape);
	m_renderTexture.display();
	const sf::Texture& texture = m_renderTexture.getTexture();

	m_sprite.setTextureRect(sf::IntRect(m_shape.getPosition().x, m_shape.getPosition().y, m_shape.getSize().x, m_shape.getSize().y));
	m_sprite.setTexture(texture);
	RequestLayout();
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
		position = m_parent->GetGlobalPosition() + m_computedPosition;
	}
	else
	{
		position = m_computedPosition;
	}
	if (!m_imageSetted)
	{

		m_renderTexture.create(m_sizeButton.x, m_sizeButton.y); // Default size, can be changed later
		m_shape.setSize(sf::Vector2f(m_sizeButton.x, m_sizeButton.y)); // Default size, can be changed later
		m_shape.setFillColor(m_normalColor);
		m_shape.setOutlineThickness(2.0f);
		m_shape.setOutlineColor(sf::Color::Black);

		m_renderTexture.draw(m_shape);
		m_renderTexture.display();
		const sf::Texture& texture = m_renderTexture.getTexture();

		m_sprite.setTextureRect(sf::IntRect(m_shape.getPosition().x, m_shape.getPosition().y, m_shape.getSize().x, m_shape.getSize().y));
		m_sprite.setTexture(texture);
	
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
	else
	{
	}

	RequestLayout();
	/*if (m_parent)
	{
		Widget::UpdateShape();
	}*/
}

void Button::ProcessInput(const sf::Event& event)
{
	const sf::Vector2f globalPosition = GetGlobalPosition();
	const sf::Vector2f size = GetSize();
	sf::FloatRect rect(globalPosition.x, globalPosition.y, size.x, size.y);

	if (event.type == sf::Event::MouseButtonPressed) 
	{
		if (rect.contains((float)event.mouseButton.x, (float)event.mouseButton.y)) 
		{
			m_pressedInside = true;
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased) 
	{
		const bool insideNow = rect.contains((float)event.mouseButton.x, (float)event.mouseButton.y);
		if (m_pressedInside && insideNow) 
		{
			OnClick(*this);
		}
		m_pressedInside = false;
	}
	else if (event.type == sf::Event::MouseMoved) 
	{
		// hover como ya tenías
		unsigned int oldState = m_state;
		m_state = None;
		const sf::Vector2f mousePosition((float)event.mouseMove.x, (float)event.mouseMove.y);
		if (rect.contains(mousePosition)) m_state = Hovered;

		if ((oldState & State::Hovered) && !(m_state & State::Hovered)) OnMouseLeave();
		else if (!(oldState & State::Hovered) && (m_state & State::Hovered)) OnMouseEnter();
	}

}
