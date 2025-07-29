#include <UI/Button.h>

Button::FuncType Button::defaultFunc = [](const sf::Event& event, Button& self)-> void {};

Button::Button(Widget* parent) : Widget(m_parent),onClick(defaultFunc), m_status(Status::None)
{
}

Button::Button(Widget* parent, FuncType OnClick) : Widget(parent), onClick(OnClick), m_status(Status::None)
{
}

Button::~Button()
{
}

bool Button::ProcessEvent(const sf::Event& event, const sf::Vector2f& parent_position)
{
	bool result = false;

	if (event.type == sf::Event::MouseButtonReleased)
	{
		const sf::Vector2f pos = m_position + parent_position;
		const sf::Vector2f size = GetSize();
		sf::FloatRect rect;
		rect.left = pos.x;
		rect.top = pos.y;
		rect.width = size.x;
		rect.height = size.y;
		if(rect.contains(event.mouseButton.x, event.mouseButton.y))
		{
			onClick(event, *this);
			result = true;
		}
	}
	else if (event.type == sf::Event::MouseMoved)
	{
		const sf::Vector2f pos = m_position + parent_position;
		const sf::Vector2f size = GetSize();
		sf::FloatRect rect;
		rect.left = pos.x;
		rect.top = pos.y;
		rect.width = size.x;
		rect.height = size.y;
		int oldStatus = m_status;
		m_status = Status::None;
		const sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
		if (rect.contains(mousePos))
		{
			m_status = Status::Hover;
		}
		if ((oldStatus & Status::Hover) && !(m_status & Status::Hover))
		{
			onMouseLeft();
		}
		else if(!(oldStatus & Status::Hover) && (m_status & Status::Hover))
		{
			onMouseEntered();
		}
	}

	return result;
}

void Button::onMouseEntered()
{
}

void Button::onMouseLeft()
{
}
