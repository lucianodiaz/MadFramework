#include <Core/QuadTree.h>
#include <ECS/Components/TransformComponent.h>

QuadTree::QuadTree(int pLevel, sf::FloatRect pBounds, std::unique_ptr<ECSManager>& ecs) : m_ecs(ecs)
{
	level = pLevel;
	m_bounds = pBounds;
	m_objects.clear();
	m_nodes = std::array<std::unique_ptr<QuadTree>, 4>();
}

void QuadTree::Clear()
{
	m_objects.clear();

	for (auto& node : m_nodes)
	{
		if (node)
		{
			node->Clear();
			node.reset();
		}
	}
}

void QuadTree::Insert(const Entity& entity)
{
	if (m_nodes[0])
	{
		int index = GetIndex(entity);

		if (index != -1) // If the entity fits in a sub-node
		{
			m_nodes[index]->Insert(entity);
			return;
		}
	}
	m_objects.push_back(entity);

	if (m_objects.size() > MAX_OBJECTS && level < MAX_LEVELS)
	{
		if (!m_nodes[0]) // If the nodes are not created yet, create them
		{
			Split();
		}

		auto it = m_objects.begin();
		while (it != m_objects.end())
		{
			int index = GetIndex(*it);
			if (index != -1)
			{
				m_nodes[index]->Insert(*it);
				it = m_objects.erase(it); // Remove from current node and continue
			}
			else
			{
				++it; // Move to the next object
			}
		}
	}
}

std::vector<Entity> QuadTree::Retrieve(const Entity& entity)
{
	std::vector<Entity> returnObjects;

	if (m_nodes[0])
	{
		int index = GetIndex(entity);
		if (index != -1)
		{
			auto subList = m_nodes[index]->Retrieve(entity);
			returnObjects.insert(returnObjects.end(), subList.begin(), subList.end());
		}
		else
		{
			
			for (const auto& node : m_nodes)
			{
				if (node)
				{
					auto subList = node->Retrieve(entity);
					returnObjects.insert(returnObjects.end(), subList.begin(), subList.end());
				}
			}
		}
	}

	// Agregar los del nodo actual
	returnObjects.insert(returnObjects.end(), m_objects.begin(), m_objects.end());

	return returnObjects;
}
void QuadTree::Split()
{
	int subWidth = m_bounds.getSize().x / 2;
	int subHeight = m_bounds.getSize().y / 2;
	int x = m_bounds.getPosition().x;
	int y = m_bounds.getPosition().y;

	m_nodes[0] = std::make_unique<QuadTree>(level + 1, sf::FloatRect(static_cast<float>(x) + subWidth, static_cast<float>(y), subWidth, subHeight),m_ecs);
	m_nodes[1] = std::make_unique<QuadTree>(level + 1, sf::FloatRect(static_cast<float>(x), static_cast<float>(y), subWidth, subHeight), m_ecs);
	m_nodes[2] = std::make_unique<QuadTree>(level + 1, sf::FloatRect(static_cast<float>(x), static_cast<float>(y) + subHeight, subWidth, subHeight), m_ecs);
	m_nodes[3] = std::make_unique<QuadTree>(level + 1, sf::FloatRect(static_cast<float>(x) + subWidth, static_cast<float>(y) + subHeight, subWidth, subHeight), m_ecs);
}

int QuadTree::GetIndex(const Entity& entity) const {
	int index = -1;
	double verticalMidpoint = m_bounds.getPosition().x + (m_bounds.width / 2);
	double horizontalMidpoint = m_bounds.getPosition().y + (m_bounds.height / 2);

	auto& collider = m_ecs->GetComponent<ColliderComponent>(entity);
	auto& transform = m_ecs->GetComponent<TransformComponent>(entity);

	// Compute AABB for the collider
	sf::FloatRect aabb;
	if (collider.shape == ColliderShape::BOX) {
		aabb = sf::FloatRect(
			transform.position.x + collider.offset.x - collider.box.width / 2,
			transform.position.y + collider.offset.y - collider.box.height / 2,
			collider.box.width,
			collider.box.height
		);
	}
	else if (collider.shape == ColliderShape::CIRCLE) {
		aabb = sf::FloatRect(
			transform.position.x + collider.offset.x - collider.circle.radius,
			transform.position.y + collider.offset.y - collider.circle.radius,
			collider.circle.radius * 2,
			collider.circle.radius * 2
		);
	}
	else if (collider.shape == ColliderShape::POLYGON) {
		// Compute AABB from transformed polygon vertices
		float minX = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::lowest();
		float minY = std::numeric_limits<float>::max();
		float maxY = std::numeric_limits<float>::lowest();

		// Transform vertices to world space
		for (const auto& point : collider.polygon.points) {
			sf::Vector2f worldPoint = transform.position + collider.offset + point;
			minX = std::min(minX, worldPoint.x);
			maxX = std::max(maxX, worldPoint.x);
			minY = std::min(minY, worldPoint.y);
			maxY = std::max(maxY, worldPoint.y);
		}
		aabb = sf::FloatRect(minX, minY, maxX - minX, maxY - minY);
	}

	bool topQuadrant = aabb.top < horizontalMidpoint && aabb.top + aabb.height < horizontalMidpoint;
	bool bottomQuadrant = aabb.top > horizontalMidpoint;

	if (aabb.left < verticalMidpoint && aabb.left + aabb.width < verticalMidpoint) {
		if (topQuadrant)
			index = 1; // Top Left
		else if (bottomQuadrant)
			index = 2; // Bottom Left
	}
	else if (aabb.left > verticalMidpoint) {
		if (topQuadrant)
			index = 0; // Top Right
		else if (bottomQuadrant)
			index = 3; // Bottom Right
	}

	return index;
}
