#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>
#include "TilemapLoader.h"
#include <MadFrameworkExport.h>
class MAD_API TilemapManager
{
public:
	TilemapManager();

	void LoadTilemap(const std::string& mapName, const std::string idLevelName);
	void SetCurrentMap(const std::string& mapName);
	void Draw(sf::RenderWindow& window);
	const std::string& GetCurrentMap() const { return m_currentMap; }

private:

	std::unordered_map<std::string, TilemapLoader> m_tilemaps;
	std::string m_currentMap;
};