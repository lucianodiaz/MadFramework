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

	m_nodes[0] = std::make_unique<QuadTree>(level + 1, sf::FloatRect(x + subWidth, y, subWidth, subHeight),m_ecs);
	m_nodes[1] = std::make_unique<QuadTree>(level + 1, sf::FloatRect(x, y, subWidth, subHeight), m_ecs);
	m_nodes[2] = std::make_unique<QuadTree>(level + 1, sf::FloatRect(x, y + subHeight, subWidth, subHeight), m_ecs);
	m_nodes[3] = std::make_unique<QuadTree>(level + 1, sf::FloatRect(x + subWidth, y + subHeight, subWidth, subHeight), m_ecs);
}

int QuadTree::GetIndex(const Entity& entity) const
{
	int index = -1;
	double verticalMidpoint = m_bounds.getPosition().x + (m_bounds.width / 2);
	double horizontalMidpoint = m_bounds.getPosition().y + (m_bounds.height / 2);

	auto& collider = m_ecs->GetComponent<ColliderComponent>(entity);
	auto& transform = m_ecs->GetComponent<TransformComponent>(entity);
	

	bool topQuadrant = transform.position.y < horizontalMidpoint &&
		transform.position.y + collider.box.height < horizontalMidpoint;

	bool bottomQuadrant = transform.position.y > horizontalMidpoint;

	if (transform.position.x < verticalMidpoint &&
		transform.position.x + collider.box.width < verticalMidpoint) // Left
	{
		if (topQuadrant)
			index = 1; // Top Left
		else if (bottomQuadrant)
			index = 2; // Bottom Left
	}
	else if (transform.position.x > verticalMidpoint) // Right
	{
		if (topQuadrant)
			index = 0; // Top Right
		else if (bottomQuadrant)
			index = 3; // Bottom Right
	}


	return index;
}
