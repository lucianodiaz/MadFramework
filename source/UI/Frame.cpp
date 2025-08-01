#include <UI/Frame.h>
#include <Core/World.h>

Frame::Frame(sf::RenderWindow& window) : Container(nullptr), ActionTarget<std::string>(World::GetWorld()->GetActionsMap()),
m_window(window)
{
}

Frame::~Frame()
{
}

void Frame::draw()
{
	m_window.draw(*this);
}

void Frame::bind(const std::string& key, const FuncType& callback)
{
	ActionTarget<std::string>::bind(key, callback);
}

void Frame::unbind(const std::string& key)
{
	ActionTarget<std::string>::unbind(key);
}

sf::Vector2f Frame::GetSize() const
{
	sf::Vector2u size = m_window.getSize();
	return sf::Vector2f(static_cast<float>(size.x), static_cast<float>(size.y));
}

void Frame::ProcessEvents()
{
	sf::Vector2f parent_position(0, 0);
	ProcessEvents(parent_position);
}

bool Frame::ProcessEvent(const sf::Event& event)
{
	sf::Vector2f parent_position(0, 0);
	return ProcessEvent(event, parent_position);
}

bool Frame::ProcessEvent(const sf::Event& event, const sf::Vector2f& parent_position)
{
	bool result = ActionTarget::processEvent(event);
	if (!result)
	{
		result = Container::ProcessEvent(event, parent_position);
	}
	return result;
}

void Frame::ProcessEvents(const sf::Vector2f& parent_position)
{
	ActionTarget::processEvents();
	Container::ProcessEvents(parent_position);
	sf::Event event;
	while(m_window.pollEvent(event)) 
	{
		Container::ProcessEvent(event, parent_position);
	}
	
}
