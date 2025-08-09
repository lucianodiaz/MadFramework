#pragma once

#include "Panel.h"

class HorizontalLayout : public Panel
{
public:
	HorizontalLayout();
	virtual ~HorizontalLayout();

	virtual sf::Vector2f GetSize()const override;

	void SetSpacing(float spacing) { m_spacing = spacing; UpdateShape(); }

	void UpdateShape() override;
protected:
	void Draw(sf::RenderWindow& window) override;

private:
	float m_spacing;

};