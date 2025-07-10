#pragma once
#include "ECS/Component.h"
#include <SFML/System/Vector2.hpp>

enum class ColliderShape {
	BOX, // Axis-aligned bounding box
	CIRCLE, // Circle collider
	POLYGON // Polygon collider
};

/*
 It can represent different shapes such as box, circle.
 It includes properties like size, offset, whether it is a trigger, and whether to draw it for debugging purposes.
 Constructors allow for easy creation of box and circle colliders with specified dimensions.
 ColliderComponent(float w, float h, bool trigger = false, bool debug = false) creates a box collider with specified width and height.
 ColliderComponent(float radius, bool trigger = false, bool debug = false) creates a circle collider with specified radius.
*/


struct ColliderComponent : public IComponent
{

	struct Polygon
	{
		std::vector<sf::Vector2f> points{}; // Vertices of the polygon
		std::vector < sf::Vector2f> transformedPoints{}; // Transformed vertices for rendering or collision detection
	};

	union
	{
		struct
		{
			float width; // Width for box collider
			float height; // Height for box collider
			float scaleWidth; // Scale for box collider
			float scaleHeight; // Scale for box collider
		} box;

		struct
		{
			float radius; // Radius for circle collider
			float scaleRadius; // Scale for circle collider
		} circle;
	};



	ColliderShape shape; // Type of collider shape

	sf::Vector2f offset{}; // Offset from the entity's position

	float rotation = 0.0f; // Rotation of the collider in degrees
	bool isStatic = false; // Whether the collider is static (does not move with the entity)
	bool isTrigger = false; // Whether the collider is a trigger
	bool isActive = true; // Whether the collider is active
	bool debugDraw = false; // Whether to draw the collider for debugging

	sf::Color debugColor = sf::Color::Red; // Color for debugging visuals

	Polygon polygon; // Polygon collider data, if applicable

	ColliderComponent() 
	{
		shape = ColliderShape::POLYGON;
		box.width = 5;
		box.height = 5;
		box.scaleWidth = 1.0f; // Default scale is 1.0, can be adjusted later
		box.scaleHeight = 1.0f; // Default scale is 1.0, can be adjusted later

		polygon.points = {
			{ -box.width / 2 , -box.height / 2}, // Top-left corner
			{box.width / 2 , -box.height / 2}, // Top-right corner
			{box.width / 2 , box.height / 2 }, // Bottom-right corner
			{-box.width / 2 , box.height / 2} // Bottom-left corner
		};
	};
	// Constructor for box collider with width and height
	ColliderComponent(float w, float h,bool trigger = false, bool debug = false)
	{
		shape = ColliderShape::POLYGON;

		box.width = w;
		box.height = h;
		box.scaleWidth = 1.0f; // Default scale is 1.0, can be adjusted later
		box.scaleHeight = 1.0f; // Default scale is 1.0, can be adjusted later
		isTrigger = trigger;
		debugDraw = debug;

		polygon.points = {
				{ -box.width / 2 , -box.height / 2}, // Top-left corner
				{box.width / 2 , -box.height / 2}, // Top-right corner
				{box.width / 2 , box.height / 2 }, // Bottom-right corner
				{-box.width / 2 , box.height / 2} // Bottom-left corner
		};
	}

	ColliderComponent(float radius, bool trigger = false, bool debug = false)
	{
		shape = ColliderShape::CIRCLE;
		circle.radius = radius;
		circle.scaleRadius = radius; // Default scale is 1.0, can be adjusted later
		isTrigger = trigger;
		debugDraw = debug;
	}

	ColliderComponent(const std::vector<sf::Vector2f>& points, bool trigger = false, bool debug = false)
	{
		shape = ColliderShape::POLYGON;
		polygon.points = points;
		isTrigger = trigger;
		debugDraw = debug;
		box.width = 1;
		box.height = 1;
		box.scaleWidth = 1; // Default scale is 1.0, can be adjusted later
		box.scaleHeight = 1; // Default scale is 1.0, can be adjusted later

	}
};