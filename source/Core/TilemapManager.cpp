#include <Core/TilemapManager.h>

TilemapManager::TilemapManager()
{
}

void TilemapManager::LoadTilemap(const std::string& mapName, const nlohmann::json& levelData)
{
	TilemapLoader loader;
	loader.LoadTilemap(mapName, levelData);
	m_tilemaps[mapName] = std::move(loader);
}

void TilemapManager::SetCurrentMap(const std::string& mapName)
{
	if (m_tilemaps.find(mapName) != m_tilemaps.end())
	{
		m_currentMap = mapName;
	}
	else
	{
		throw std::runtime_error("Tilemap not found: " + mapName);
	}
}

void TilemapManager::Draw(sf::RenderWindow& window)
{
	if (!m_currentMap.empty())
	{
		m_tilemaps[m_currentMap].Draw(window);
	}
}
