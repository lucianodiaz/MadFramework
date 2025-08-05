#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
#include <vector>
#include "Widget.h"


class UserWidget
{
public:
	UserWidget();
	virtual ~UserWidget() = default;

	virtual void OnConstruct() {};
	virtual void Update(float deltaTime) = 0;

	/// <summary>
	/// Sets the main widget for the user interface.
	/// You can only have one main widget.
	/// </summary>
	/// <param name="mainWidget"></param>
	void SetMainWidget(std::shared_ptr<Widget> mainWidget);

	/// <summary>
	/// Adds a child widget to the main widget.
	/// </summary>
	/// <param name="childWidget"></param>
	void AddChildWidget(std::shared_ptr<Widget> childWidget);

	std::shared_ptr<Widget>& GetMainWidget() { return m_mainWidget; }

	template<typename T, typename... Args>
	std::shared_ptr<T>& CreateWidget(Args&&... args);

	//Only have ONE WIDGET CHILDREN
	//  Canvas Panel or Layout 
	// also can add Buttons Labels Images etc...
	//But only one main widget
	//Then you can add children to main widget and children to children etc...

private:
	friend class SceneManager;
	void Draw(sf::RenderWindow& window);//Only father Draw in window 
	void ProcessInput(const sf::Event& event);// Process input events for the main widget

	std::shared_ptr<Widget> m_mainWidget; // Main widget that contains all other widgets only exist one main widget
};

template<typename T, typename ...Args>
inline std::shared_ptr<T>& UserWidget::CreateWidget(Args && ...args)
{
	auto widget = std::make_shared<T>(std::forward<Args>(args)...);

	if(m_mainWidget)
	{
		m_mainWidget->AddChild(widget);
	}
	else
	{
		m_mainWidget = widget;
		//throw std::runtime_error("Main widget is not set. Please set a main widget before adding child widgets.");
	}

	return widget;
}
