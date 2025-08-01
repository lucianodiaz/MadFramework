#pragma once
#include <SFML/Graphics.hpp>
#include "Widget.h"


class Layout : protected Widget
{
public:
	Layout(Widget* parent = nullptr);
	virtual ~Layout();

	void SetSpace(float pixels);

protected:
	friend class Container;
	float m_space;
};