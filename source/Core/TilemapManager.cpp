#include <Core/TilemapManager.h>
#include <Core/World.h>

TilemapManager::TilemapManager()
{
}

void TilemapManager::LoadTilemap(const std::string& mapName, const std::string idLevelName)
{
	TilemapLoader loader;
	auto& levelData = World::GetWorld()->GetJson(idLevelName);
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
