#include <UI/Image.h>


Image::Image()
{
}

Image::~Image()
{
}


void Image::SetImage(const sf::Texture& texture)
{
	m_texture = texture;
	m_sprite.setTexture(m_texture);
	UpdateShape();
}

sf::Vector2f Image::GetSize() const
{
	return sf::Vector2f(m_texture.getSize());
}

void Image::Draw(sf::RenderWindow& window)
{

	if (!m_visible)
		return;
	sf::RenderStates states;
	sf::Vector2f position;
	if (m_parent)
	{
		position = m_parent->GetGlobalPosition() + m_computedPosition;
	}
	else
	{
		position = m_computedPosition;
	}
	states.transform.translate(position);
	window.draw(m_sprite, states);
}