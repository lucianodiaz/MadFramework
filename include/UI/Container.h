#pragma once
#include "Widget.h"
#include "Layout.h"
class Container : public Widget
{
public:
	Container(Widget* parent = nullptr);
	virtual ~Container();
	
	void SetLayout(Layout* layout);
	Layout* GetLayout() const;

	virtual sf::Vector2f GetSize() const override;

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual bool ProcessEvent(const sf::Event& event, const sf::Vector2f& parent_position) override;
	virtual void ProcessEvents(const sf::Vector2f& parent_position) override;

private:
	Layout* m_layout;
};

