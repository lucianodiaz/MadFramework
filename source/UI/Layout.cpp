#include <UI/Layout.h>

Layout::Layout(Widget* parent) : Widget(parent), m_space(5.0f)
{
}

Layout::~Layout()
{
}

void Layout::SetSpace(float pixels)
{
	if (pixels >= 0)
	{
		m_space = pixels;
		UpdateShape();
	}
	else
	{
		throw std::invalid_argument("Space cannot be negative");
	}
}