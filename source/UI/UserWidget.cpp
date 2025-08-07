#include <UI/UserWidget.h>
#include <Core/World.h>

UserWidget::UserWidget()
{
}

void UserWidget::SetMainWidget(std::shared_ptr<Widget> mainWidget)
{
	m_mainWidget = mainWidget;
}

void UserWidget::AddChildWidget(std::shared_ptr<Widget> childWidget)
{
	if(m_mainWidget)
	{
		m_mainWidget->AddChild(childWidget);
	}
	else
	{
		throw std::runtime_error("Main widget is not set. Please set a main widget before adding child widgets.");
	}
}

void UserWidget::ProcessInput(const sf::Event& event)
{
	if (m_mainWidget)
	{
		m_mainWidget->ProcessInput(event);
		
		for (const auto& child : m_mainWidget->m_children)
		{
			if (child->IsVisible())
			{
				child->ProcessInput(event);
			}
		}
	}
}

void UserWidget::Draw(sf::RenderWindow& window)
{
	if (m_mainWidget)
	{
		m_mainWidget->Draw(window);
		
		for (const auto& child : m_mainWidget->m_children)
		{
			if (child->IsVisible())
			{
				child->Draw(window);
			}
		}
	}
	
}
