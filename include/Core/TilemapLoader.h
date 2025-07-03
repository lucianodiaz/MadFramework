#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <nlohmann/json.hpp>

struct TileSet
{
	int firstgid; // First global tile ID
	int tileWidth; // Width of each tile in pixels
	int tileHeight; // Height of each tile in pixels
	std::string source; // Path to the tile set image file
	sf::Texture texture; // Texture object for the tile set image
	int tileCount; // Total number of tiles in the tile set
	int columns; // Number of columns in the tile set image
	std::unordered_map<int, sf::FloatRect> colliderByTileID; // Map of tile IDs to their collision rectangles
	std::string textureName;
};

struct Layer 
{
	std::string name; // Name of the layer
	std::vector<int> data; // Tile IDs for the layer
	int width; // Width of the layer in tiles
	int height; // Height of the layer in tiles
};

class TilemapLoader
{
public:

	void LoadTilemap(std::string mapName, const nlohmann::json& levelData);

	void Draw(sf::RenderWindow& window);

private:
	std::vector < sf::VertexArray> m_Vertexlayers;
	std::unordered_map<int, sf::Texture*> m_tileSetTextures;

	std::unordered_map<std::string,std::vector<Layer>> m_layers;
	std::unordered_map<std::string,std::vector<TileSet>> m_tileSets;

};