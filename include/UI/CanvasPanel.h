#pragma once
#include "Panel.h"

class CanvasPanel : public Panel
{
public:
	CanvasPanel();
	virtual ~CanvasPanel();
	virtual sf::Vector2f GetSize() const override;
	
protected:
	void Draw(sf::RenderWindow& window) override;
	void UpdateShape() override;
};