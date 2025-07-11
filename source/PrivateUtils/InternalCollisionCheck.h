#pragma once

#include <cmath>
#include <utility>
#include <ECS/Components/TransformComponent.h>
#include <ECS/Components/ColliderComponent.h>
#include <SFML/System/Vector2.hpp>
namespace MAD
{
	namespace CollisionUtils
	{

		struct Projection
		{
			float min;
			float max;
		};

		Projection projectPolygon(const std::vector<sf::Vector2f>& vertices, const sf::Vector2f& axis)
		{
			float min = std::numeric_limits<float>::max();
			float max = std::numeric_limits<float>::lowest();

			for (const auto& vertex : vertices)
			{
				float dot = vertex.x * axis.x + vertex.y * axis.y;
				min = std::min(min, dot);
				max = std::max(max, dot);
			}

			return { min, max };
		}

		sf::Vector2f getEdgeNormal(const sf::Vector2f& p1, const sf::Vector2f& p2)
		{
			sf::Vector2f edge = p2 - p1;
			return sf::Vector2f(-edge.y, edge.x); // Perpendicular vector (normal)
		}

		bool projectionOverlap(const Projection& p1, const Projection& p2)
		{
			return p1.max >= p2.min && p2.max >= p1.min;
		}

		std::vector<sf::Vector2f> getTransformedVertices(const ColliderComponent& collider, const TransformComponent& transform)
		{
			std::vector<sf::Vector2f> vertices = collider.polygon.points;
			std::vector<sf::Vector2f> transformedVertices;
			float rad = collider.rotation * 3.14159f / 180.0f; // Convert degrees to radians
			float cosR = std::cos(rad);
			float sinR = std::sin(rad);

			for (const auto& vertex : vertices)
			{
				float x = vertex.x * collider.box.scaleWidth * cosR - vertex.y * collider.box.scaleHeight * sinR;
				float y = vertex.x * collider.box.scaleWidth * sinR + vertex.y * collider.box.scaleHeight * cosR;
				transformedVertices.push_back(transform.position + collider.offset + sf::Vector2f(x, y));
			}

			return transformedVertices;
		}


		bool SATCollision(const ColliderComponent& colliderA, const TransformComponent& transformA,
			const ColliderComponent& colliderB, const TransformComponent& transformB, sf::Vector2f& mtv)
		{
			auto verticesA = getTransformedVertices(colliderA, transformA);
			auto verticesB = getTransformedVertices(colliderB, transformB);

			float minOverlap = std::numeric_limits<float>::max();
			sf::Vector2f minAxis;

			//test axes from polygon A
			for (size_t i = 0; i < verticesA.size(); ++i)
			{
				sf::Vector2f p1 = verticesA[i];
				sf::Vector2f p2 = verticesA[(i + 1) % verticesA.size()];
				sf::Vector2f axis = getEdgeNormal(p1, p2);
				float len = std::sqrt(axis.x * axis.x + axis.y * axis.y);

				if (len > 0) axis /= len; // Normalize the axis

				Projection projectionA = projectPolygon(verticesA, axis);
				Projection projectionB = projectPolygon(verticesB, axis);

				if (!projectionOverlap(projectionA, projectionB))
				{
					return false;
				}

				float overlap = std::min(projectionA.max, projectionB.max) - std::max(projectionA.min, projectionB.min);

				if (overlap < minOverlap)
				{
					minOverlap = overlap;
					minAxis = axis;
				}
			}

			//test axes from polygon B
			for (size_t i = 0; i < verticesB.size(); ++i)
			{
				sf::Vector2f p1 = verticesB[i];
				sf::Vector2f p2 = verticesB[(i + 1) % verticesB.size()];
				sf::Vector2f axis = getEdgeNormal(p1, p2);
				float len = std::sqrt(axis.x * axis.x + axis.y * axis.y);

				if (len > 0) axis /= len; // Normalize the axis

				Projection projectionA = projectPolygon(verticesA, axis);
				Projection projectionB = projectPolygon(verticesB, axis);

				if (!projectionOverlap(projectionA, projectionB))
				{
					return false;
				}

				float overlap = std::min(projectionA.max, projectionB.max) - std::max(projectionA.min, projectionB.min);

				if (overlap < minOverlap)
				{
					minOverlap = overlap;
					minAxis = axis;
				}
			}

			// Calculate the minimum translation vector (MTV)

			sf::Vector2f direction = transformB.position - transformA.position;
			if (direction.x * minAxis.x + direction.y * minAxis.y < 0) {
				minAxis = -minAxis; // Ensure MTV points from A to B
			}
			mtv = minAxis * minOverlap;

			return true; // Collision detected
		}

		// Circle-Polygon collision
		bool CirclePolygonCollision(const ColliderComponent& circleCollider, const TransformComponent& circleTransform,
			const ColliderComponent& polyCollider, const TransformComponent& polyTransform,
			sf::Vector2f& mtv) 
		{
			auto vertices = getTransformedVertices(polyCollider, polyTransform);
			sf::Vector2f circleCenter = circleTransform.position + circleCollider.offset;
			float radius = circleCollider.circle.radius;

			float minOverlap = std::numeric_limits<float>::max();
			sf::Vector2f minAxis;

			// Test polygon edge normals
			for (size_t i = 0; i < vertices.size(); ++i) {
				sf::Vector2f p1 = vertices[i];
				sf::Vector2f p2 = vertices[(i + 1) % vertices.size()];
				sf::Vector2f axis = getEdgeNormal(p1, p2);
				float len = std::sqrt(axis.x * axis.x + axis.y * axis.y);
				if (len > 0) axis /= len;

				Projection projPoly = projectPolygon(vertices, axis);
				float circleDot = circleCenter.x * axis.x + circleCenter.y * axis.y;
				Projection projCircle = { circleDot - radius, circleDot + radius };

				if (!projectionOverlap(projPoly, projCircle)) {
					return false;
				}

				float overlap = std::min(projPoly.max, projCircle.max) - std::max(projPoly.min, projCircle.min);
				if (overlap < minOverlap) {
					minOverlap = overlap;
					minAxis = axis;
				}
			}

			// Test axis from circle center to closest polygon vertex
			sf::Vector2f closestVertex;
			float minDist = std::numeric_limits<float>::max();
			for (const auto& vertex : vertices) {
				float dist = std::hypot(vertex.x - circleCenter.x, vertex.y - circleCenter.y);
				if (dist < minDist) {
					minDist = dist;
					closestVertex = vertex;
				}
			}
			sf::Vector2f axis = circleCenter - closestVertex;
			float len = std::sqrt(axis.x * axis.x + axis.y * axis.y);
			if (len > 0) axis /= len;

			Projection projPoly = projectPolygon(vertices, axis);
			float circleDot = circleCenter.x * axis.x + circleCenter.y * axis.y;
			Projection projCircle = { circleDot - radius, circleDot + radius };

			if (!projectionOverlap(projPoly, projCircle)) {
				return false;
			}

			float overlap = std::min(projPoly.max, projCircle.max) - std::max(projPoly.min, projCircle.min);
			if (overlap < minOverlap) {
				minOverlap = overlap;
				minAxis = axis;
			}

			// Compute MTV direction
			sf::Vector2f direction = circleCenter - closestVertex;
			if (direction.x * minAxis.x + direction.y * minAxis.y < 0) {
				minAxis = -minAxis;
			}
			mtv = minAxis * minOverlap;
			return true;
		}

		// Utility functions for collision detection can be added here

		bool AABBCollision(const sf::Vector2f& posA, float widthA, float heightA,
			const sf::Vector2f& posB, float widthB, float heightB) {
			return posA.x < posB.x + widthB &&
				posA.x + widthA > posB.x &&
				posA.y < posB.y + heightB &&
				posA.y + heightA > posB.y;
		}

		bool CircleCollision(const sf::Vector2f& posA, float radiusA,
			const sf::Vector2f& posB, float radiusB,sf::Vector2f& mtv) 
		{

			//float dist = std::hypot(posA.x - posB.x, posA.y - posB.y);

			float distX = posA.x - posB.x;// normal.x
			float distY = posA.y - posB.y; // normal.y
			float distance = sqrt(distX * distX + distY * distY); // magnitude

			auto normal = (sf::Vector2f(distX, distY)) / distance; //normalize magnitude

			mtv = (normal * ((radiusA + radiusB) - distance)) / 2.0f;

			return distance < radiusA + radiusB;
		}

		bool BoxCircleCollision(const sf::Vector2f& boxPos, float boxWidth, float boxHeight,
			const sf::Vector2f& circlePos, float radius) {
			float closestX = std::max(boxPos.x, std::min(circlePos.x, boxPos.x + boxWidth));
			float closestY = std::max(boxPos.y, std::min(circlePos.y, boxPos.y + boxHeight));
			float distanceX = circlePos.x - closestX;
			float distanceY = circlePos.y - closestY;
			return (distanceX * distanceX + distanceY * distanceY) < (radius * radius);
		}
	}
}
