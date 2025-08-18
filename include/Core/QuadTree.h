#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <ECS/Components/ColliderComponent.h>
#include <ECS/Entity.h>


class ECSManager;

class QuadTree
{
public:
	QuadTree(int pLevel, sf::FloatRect pBounds, std::unique_ptr<ECSManager>& ecs);

	void SetBounds(const sf::FloatRect& bounds) {m_bounds = bounds; }

	void Clear();
	void Insert(const Entity& entity);
	std::vector<Entity> Retrieve(const Entity& entity);

	~QuadTree() = default;
private:

	void Split();

	int GetIndex(const Entity& entity) const;

	const int MAX_OBJECTS = 8;
	const int MAX_LEVELS = 5;

	int level;
	std::vector<Entity> m_objects;
	sf::FloatRect m_bounds;
	std::array<std::unique_ptr<QuadTree>, 4> m_nodes;
	std::unique_ptr<ECSManager>& m_ecs;
};

