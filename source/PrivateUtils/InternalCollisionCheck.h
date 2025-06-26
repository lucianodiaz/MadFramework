#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <utility>


inline bool AABBColision(const sf::Vector2f& posA, float wA, float hA,
	const sf::Vector2f& posB, float wB, float hB)
{
	return (std::abs(posA.x - posB.x) * 2 < (wA + wB)) &&
		(std::abs(posA.y - posB.y) * 2 < (hA + hB));
}

inline bool CircleCollision(const sf::Vector2f& posA, float radiusA,
	const sf::Vector2f& posB, float radiusB)
{
	float dx = posA.x - posB.x;
	float dy = posA.y - posB.y;
	float distanceSquared = dx * dx + dy * dy;
	float radiusSum = radiusA + radiusB;
	return distanceSquared < (radiusSum * radiusSum);
}

inline bool BoxCircleCollision(const sf::Vector2f& boxPos, float boxWidht, float boxHeight,
	const sf::Vector2f& circlePos, float radius)
{

	float closestX = std::max(boxPos.x - boxWidht / 2, std::min(circlePos.x, boxPos.x + boxWidht / 2));
	float closestY = std::max(boxPos.y - boxHeight / 2, std::min(circlePos.y, boxPos.y + boxHeight / 2));

	float dx = circlePos.x - closestX;
	float dy = circlePos.y - closestY;

	return (dx * dx + dy * dy) < (radius * radius);
}