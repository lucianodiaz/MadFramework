#include <UI/VerticalLayout.h>


VerticalLayout::VerticalLayout(Widget* parent) : Layout(parent)
{
}

VerticalLayout::~VerticalLayout()
{
	for(Widget* widget : m_widgets)
	{
		if(widget->GetParent() == this)
		{
			delete widget;
		}
	}
}

Widget* VerticalLayout::at(unsigned int index) const
{
	if (index < m_widgets.size())
	{
		return m_widgets.at(index);
	}
	else
	{
		throw std::out_of_range("Index out of range");
	}
}

void VerticalLayout::AddWidget(Widget* widget)
{
	widget->SetParent(this);
	m_widgets.emplace_back(widget);
	UpdateShape();

}

sf::Vector2f VerticalLayout::GetSize() const
{
	float max_x = 0;
	float y = 0;
	for(Widget* widget : m_widgets)
	{
		sf::Vector2f size = widget->GetSize();
		if (size.x > max_x)
		{
			max_x = size.x;
			y += size.y + m_space; // Add space between widgets
		}
		
	}
	return sf::Vector2f(max_x + m_space * 2, y + m_space); // Add space on both sides
}

bool VerticalLayout::ProcessEvent(const sf::Event& event, const sf::Vector2f& parent_position)
{
	for (Widget* widget : m_widgets)
	{
		if (widget->ProcessEvent(event, parent_position + m_position))
		{
			return true;
		}
	}
	return false;
}
void VerticalLayout::ProcessEvents(const sf::Vector2f& parent_position)
{
	for (Widget* widget : m_widgets)
	{
		widget->ProcessEvents(parent_position + m_position); // Pass the parent's position to each widget
	}
}

void VerticalLayout::UpdateShape()
{
	float max_x = (m_parent ? m_parent->GetSize().x : 0);
	for (Widget* widget : m_widgets)
	{
		sf::Vector2f size = widget->GetSize();
		float widget_x = size.x;

		if (widget_x > max_x)
			max_x = widget_x;
	}

	float pos_y = m_space;
	if (m_parent)
		pos_y = (m_parent->GetSize().y - GetSize().y) / 2.f;

	for (Widget* widget : m_widgets)
	{
		sf::Vector2f size = widget->GetSize();
		widget->SetPosition((max_x - size.x) / 2.0, pos_y);
		pos_y += size.y + m_space;
	}

	Widget::UpdateShape();
}

void VerticalLayout::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const Widget* widget : m_widgets)
	{
		target.draw(*widget, states);
	}
	
	// Optionally draw a border or background for the layout itself
	// target.draw(m_shape, states); // Uncomment if you have a shape to draw
}