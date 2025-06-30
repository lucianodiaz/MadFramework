#pragma once
#include <SFML/Graphics.hpp>
#include <ECS/Component.h>

struct ShakeEffect
{
	float intensity{}; // The strength of the shake effect
	float duration{};  // How long the shake effect lasts
	float elapsed{};   // Time elapsed since the shake effect started
};

struct Bounds
{
	sf::FloatRect bounds{}; // The bounding rectangle for the camera
};
struct CameraViewComponent : public IComponent
{
	CameraViewComponent(sf::FloatRect s) : cameraView(sf::View(s))
	{
		size = sf::Vector2f(s.width, s.height);
	}

	CameraViewComponent(sf::Vector2f c, sf::Vector2f s) : cameraView(sf::View(c,s))
	{
		center = c;
		size = s;
	}
	sf::View cameraView;
	sf::Vector2f center;
	sf::Vector2f size;
	sf::Vector2f offset{};
	sf::Vector2f cameraPosition;

	ShakeEffect shakeEffect{};// The shake effect parameters
	Bounds boundsLimit{};// The bounding rectangle for the camera

	float zoom = 1.0f;
	float Angle = 0.0f;
	float LagFactor = 1.0f;
	bool follorTarget = true;
	bool isMainCamera = true;

	bool isShake = false; // if true, camera will shake when shakeEffect is applied
	bool clampToBounds = false; // if true, camera will not go outside the bounds defined in boundsLimit

};