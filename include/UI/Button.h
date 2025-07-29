#pragma once
#include "Widget.h"
#include <functional>


class Button : public Widget
{
public:
	using FuncType = std::function<void(const sf::Event& event, Button& self)>;

	static FuncType defaultFunc;

	Button(Widget* parent = nullptr);
	Button(Widget* parent, FuncType OnClick);

	virtual ~Button();

	FuncType onClick;

protected:
	virtual bool ProcessEvent(const sf::Event& event, const sf::Vector2f& parent_position) override;

	virtual void onMouseEntered();

	virtual void onMouseLeft();

private:
	enum Status {None = 0, Hover = 1};
	int m_status;
};

