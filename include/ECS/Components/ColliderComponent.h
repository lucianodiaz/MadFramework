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
	ColliderComponent() 
	{
		shape = ColliderShape::BOX;
		box.width = 5;
		box.height = 5;
	};
	// Constructor for box collider with width and height
	ColliderComponent(float w, float h,bool trigger = false, bool debug = false)
	{
		shape = ColliderShape::BOX;
		box.width = w;
		box.height = h;
		isTrigger = trigger;
		debugDraw = debug;
	}

	ColliderComponent(float radius, bool trigger = false, bool debug = false)
	{
		shape = ColliderShape::CIRCLE;
		circle.radius = radius;
		isTrigger = trigger;
		debugDraw = debug;
	}

	ColliderShape shape; // Type of collider shape

	sf::Vector2f offset{}; // Offset from the entity's position

	union {
		struct
		{
			float width; // Width for box collider
			float height; // Height for box collider
		} box;

		struct
		{
			float radius; // Radius for circle collider
		} circle;
	};

	bool isTrigger = false; // Whether the collider is a trigger
	bool isActive = true; // Whether the collider is active
	bool debugDraw = false; // Whether to draw the collider for debugging
};