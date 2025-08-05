#pragma once
#include "Widget.h"
#include <functional>

class Button : public Widget
{
public:

	using FunctionType = std::function<void()>;

	static FunctionType DefaultOnClickFunction;

	Button();
	virtual ~Button();

	virtual sf::Vector2f GetSize() const override;

	FunctionType OnClick;

protected:
	virtual void Draw(sf::RenderWindow& window) override;
	void OnMouseEnter();
	void OnMouseLeave();

	virtual void UpdateShape() override;
	virtual void ProcessInput(const sf::Event& event) override;

	sf::RenderTexture m_renderTexture; // Render texture for the button
	sf::RectangleShape m_shape; // Shape of the button, used for hit testing
	sf::Texture m_texture; // Texture for the button
	sf::Sprite m_sprite; // Sprite for the button


	sf::Color m_normalColor = sf::Color(200, 200, 200, 255);
	sf::Color m_hoveredColor = sf::Color(255, 255, 255, 255);
private:
	enum State
	{
		None,
		Hovered
	};

	unsigned int m_state = None; // Current state of the button


	bool m_imageSetted = false; // Flag to check if the image is set

	float m_margin = 10.0f;
};