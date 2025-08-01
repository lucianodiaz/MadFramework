#pragma once
#include "Layout.h"

class VerticalLayout : public Layout
{
public:
	VerticalLayout(const VerticalLayout&) = delete;
	VerticalLayout& operator=(const VerticalLayout&) = delete;
	VerticalLayout(Widget* parent = nullptr);
	virtual ~VerticalLayout();

	void AddWidget(Widget* widget);

	Widget* at(unsigned int index)const;
	virtual sf::Vector2f GetSize() const override;

protected:

	virtual bool ProcessEvent(const sf::Event& event, const sf::Vector2f& parent_position) override;
	virtual void ProcessEvents(const sf::Vector2f& parent_position) override;

private:
	std::vector<Widget*> m_widgets;
	virtual void UpdateShape() override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

};