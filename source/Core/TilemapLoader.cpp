#include <Core/TilemapLoader.h>
#include <Core/World.h>
#include <ECS/Components/ColliderComponent.h>
#include <ECS/Components/TransformComponent.h>
#include <Gameplay/Actor.h>

void TilemapLoader::LoadTilemap(std::string mapName, const nlohmann::json& levelData)
{

	for (const auto& layer : levelData["layers"])
	{
		if (layer["type"] == "tilelayer")
		{
			Layer l;
			l.name = layer["name"];
			l.width = layer["width"];
			l.height = layer["height"];
			l.data = layer["data"].get<std::vector<int>>();
			m_layers[mapName].emplace_back(l);
		}

		if (layer["type"] == "objectgroup")
		{

			for (const auto& object : layer["objects"])
			{
				float x = object["x"];
				float y = object["y"];
				std::string name = object["name"];
				auto& actor = World::GetWorld()->SpawnActor<Actor>(x, y);
				actor.SetGameTag(name);
			}
		}
	}

	for (const auto& tilesetEntry : levelData["tilesets"])
	{
		std::string tilesetPath = tilesetEntry["source"];
		int firstGid = tilesetEntry["firstgid"];
		auto const srcAsset = "assets/";//World::GetWorld()->AssetsPath();

		std::string jsonPath = srcAsset + tilesetPath;
		std::ifstream file(jsonPath);

		if (!file.is_open())
		{
			throw std::runtime_error("Error opening file: " + jsonPath);
		}

		nlohmann::json tilesetJson;
		file >> tilesetJson;

		TileSet tileSet;

		tileSet.firstgid = firstGid;
		tileSet.tileWidth = tilesetJson["tilewidth"];
		tileSet.tileHeight = tilesetJson["tileheight"];
		tileSet.source = tilesetJson["image"]; //source image
		tileSet.textureName = tilesetJson["name"];
		tileSet.tileCount = tilesetJson["tilecount"];
		tileSet.columns = tilesetJson["columns"];

		sf::VertexArray layerVA;
		layerVA.setPrimitiveType(sf::Quads);
		layerVA.resize(tileSet.tileCount * 4); // Each tile has 4 vertices
		m_Vertexlayers.emplace_back(layerVA);

		World::GetWorld()->LoadTexture( tileSet.textureName, srcAsset + tileSet.source);

		tileSet.texture = World::GetWorld()->GetTexture(tileSet.textureName);
		
		if (tilesetJson.contains("tiles"))
		{
			for (const auto& tile : tilesetJson["tiles"])
			{
				int id = tile["id"];

				if (tile.contains("objectgroup"))
				{
					const auto& obj = tile["objectgroup"]["objects"][0];
					sf::FloatRect collider;
					collider.left = obj["x"];
					collider.top = obj["y"];
					collider.width = obj["width"];
					collider.height = obj["height"];
					tileSet.colliderByTileID[id] = collider;
					//collider.top += tileSet.tileHeight; // Adjust the top position to match the tile's origin
				}
				m_tileSetTextures[id] = &tileSet.texture; // Store the texture pointer for each tile ID
			}
		}
		m_tileSets[mapName].emplace_back(tileSet);
	}

	for (const auto & [mapName, layers] : m_layers) {
		for (size_t i = 0; i < layers.size(); ++i) {
			const auto& layer = layers[i];
			auto& vertexArray = m_Vertexlayers[i];
			vertexArray.setPrimitiveType(sf::Quads);
			vertexArray.resize(layer.width * layer.height * 4);

			//const auto& tileSet = m_tileSets[mapName][0]; // Suponiendo un solo tileset por ahora
			for (const auto& tileSet : m_tileSets[mapName])
			{
				for (int y = 0; y < layer.height; ++y) {
					for (int x = 0; x < layer.width; ++x) {
						int tileIndex = x + y * layer.width;
						int tileID = layer.data[tileIndex];

						//if (tileID == 0) continue; // 0 = sin tile

						tileID -= tileSet.firstgid;

						int tu = tileID % tileSet.columns;
						int tv = tileID / tileSet.columns;

						sf::Vertex* quad = &vertexArray[tileIndex * 4];

						// Posiciones
						quad[0].position = sf::Vector2f(static_cast<float>(x) * tileSet.tileWidth, static_cast<float>(y) * tileSet.tileHeight);
						quad[1].position = sf::Vector2f((static_cast<float>(x) + 1) * tileSet.tileWidth, static_cast<float>(y) * tileSet.tileHeight);
						quad[2].position = sf::Vector2f((static_cast<float>(x) + 1) * tileSet.tileWidth, (static_cast<float>(y) + 1) * tileSet.tileHeight);
						quad[3].position = sf::Vector2f(static_cast<float>(x) * tileSet.tileWidth, (static_cast<float>(y) + 1) * tileSet.tileHeight);

						// Coordenadas UV
						quad[0].texCoords = sf::Vector2f(static_cast<float>(tu) * tileSet.tileWidth, static_cast<float>(tv) * tileSet.tileHeight);
						quad[1].texCoords = sf::Vector2f((static_cast<float>(tu) + 1) * tileSet.tileWidth, static_cast<float>(tv) * tileSet.tileHeight);
						quad[2].texCoords = sf::Vector2f((static_cast<float>(tu) + 1) * tileSet.tileWidth, (static_cast<float>(tv) + 1) * tileSet.tileHeight);
						quad[3].texCoords = sf::Vector2f(static_cast<float>(tu) * tileSet.tileWidth, (static_cast<float>(tv) + 1) * tileSet.tileHeight);


						// Si hay colision, agregarla al mapa de colisiones
						if (tileSet.colliderByTileID.find(tileID) != tileSet.colliderByTileID.end()) {
							auto rect = tileSet.colliderByTileID.at(tileID);


							float worldX = x * tileSet.tileWidth + rect.left;
							float worldY = (y * tileSet.tileHeight + rect.top);

							auto& tileActor = World::GetWorld()->SpawnActor<Actor>(worldX, worldY);
							tileActor.SetGameTag("TilemapCollider");
							tileActor.AddComponent<ColliderComponent>(rect.width, rect.height, false, false);

							ColliderComponent& collider = tileActor.GetComponent<ColliderComponent>();

							auto& transform = tileActor.GetComponent<TransformComponent>();
							auto centerX = rect.width / 2.f;
							auto centerY = rect.height / 2.f;

							collider.offset.x = centerX;
							collider.offset.y = centerY;
							collider.isStatic = true;
						}

					}
				}
			}
			
		}
	}
}

void TilemapLoader::Draw(sf::RenderWindow& window)
{
	for (size_t i = 0; i < m_Vertexlayers.size(); ++i)
	{

		for (auto tileSet : m_tileSets.begin()->second)
		{
			window.draw(m_Vertexlayers[i], &tileSet.texture);

		}
	//	const auto& tileSet = m_tileSets.begin()->second[0]; // Suponiendo 1 tileset por ahora
		
	}
}
