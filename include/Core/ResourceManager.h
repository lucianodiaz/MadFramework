#pragma once
#include <unordered_map>
#include <memory>
#include <iostream>
#include <SFML/Audio.hpp>
#include <nlohmann/json.hpp>
#include "Utils/StringUtil.h"
#include <fstream>


template<typename RESOURCE, typename ID=int>
class ResourceManager
{
public:

	ResourceManager(const ResourceManager&) = delete; // no copy

	ResourceManager& operator=(const ResourceManager&) = delete; // no copy

	ResourceManager() = default;

	template<typename... Args>
	void Load(const ID& id, Args&& ... args);

	void Unload(const ID& id);

	void Clear();

	RESOURCE& Get(const ID& id)const;


	void LoadFromJson(const std::string& jsonPath);
private:
	std::unordered_map<ID, std::unique_ptr<RESOURCE>> m_resources;

};


template<typename RESOURCE, typename ID>
template<typename ...Args>
inline void ResourceManager<RESOURCE, ID>::Load(const ID& id, Args && ...args)
{
	std::unique_ptr<RESOURCE> ptr = std::make_unique<RESOURCE>(); //(new RESOURCE);

	if (!ptr->loadFromFile(std::forward<Args>(args)...))
	{
		throw std::runtime_error("Error loading resource");
	}

	if (m_resources.emplace(id, std::move(ptr)).second == false)
	{
		throw std::runtime_error("Error inserting resource, is already load?");
	}

	std::cout << "Resource loaded: " << id << std::endl;
}

template<typename RESOURCE, typename ID>
inline void ResourceManager<RESOURCE, ID>::Unload(const ID& id)
{
	auto it = m_resources.find(id);
	if (it == m_resources.end())
	{
		throw std::runtime_error("Resource with ID not found: " + std::to_string(id));
	}
	m_resources.erase(it);
	std::cout << "Resource unloaded: " << id << std::endl;
}

template<typename RESOURCE, typename ID>
inline void ResourceManager<RESOURCE, ID>::Clear()
{
	m_resources.clear();
	std::cout << "All resources cleared" << std::endl;
}

template<typename RESOURCE, typename ID>
inline RESOURCE& ResourceManager<RESOURCE, ID>::Get(const ID& id) const
{
	auto it = m_resources.find(id);
	if (it == m_resources.end())
	{
		throw std::runtime_error("Resource with ID not found");
	}
	return *(it->second);
}

template<typename RESOURCE, typename ID>
inline void ResourceManager<RESOURCE, ID>::LoadFromJson(const std::string& jsonPath)
{
	std::ifstream file(jsonPath);

	if (!file.is_open())
	{
		throw std::runtime_error("Error opening file: " + jsonPath);
	}

	nlohmann::json jsonData;

	file >> jsonData;

	for (auto& resource : jsonData)
	{

		std::string type = StringUtil::ToLower(resource["type"]);
		std::string src = "";
		if (type != "shader")
		{
			src = resource["src"];
		}
		
		std::string id = resource["id"];
	

		if constexpr (std::is_same<RESOURCE, sf::Texture>::value)
		{
			if (type == "texture")
			{
				Load(id, src);
			}

		}
		else if constexpr (std::is_same<RESOURCE, sf::Font>::value)
		{
			if (type == "font")
			{
				Load(id, src);
			}
		
		}
		else if constexpr (std::is_same<RESOURCE, sf::SoundBuffer>::value)
		{
			if (type == "sound")
			{
				Load(id, src);
			}
		}
		else if constexpr (std::is_same<RESOURCE, nlohmann::json>::value)
		{
			if (type == "json")
			{
				std::ifstream jsonFile(src);
				if (!jsonFile.is_open())
				{
					throw std::runtime_error("Error opening file: " + src);
				}

				nlohmann::json jsonContent;
				jsonFile >> jsonContent;

				auto ptr = std::make_unique<nlohmann::json>(jsonContent);
				m_resources.emplace(id, std::move(ptr));
			}
		}
		else if constexpr (std::is_same<RESOURCE, sf::Shader>::value)
		{
			if (type == "shader")
			{
				// Soporta: solo fragment, o vertex+fragment
				if (resource.contains("vertex") && resource.contains("fragment")) {
					std::string v = resource["vertex"];
					std::string f = resource["fragment"];
					Load(id, v, f);
				}
				else if (resource.contains("fragment")) {
					std::string f = resource["fragment"];
					Load(id, f, sf::Shader::Fragment);
				}
				else {
					throw std::runtime_error("Shader resource needs 'fragment' or ('vertex' and 'fragment'): " + id);
				}
			}
		}
		else
		{
			throw std::runtime_error("Resource type not supported: " + type);
		}
	}

}



//Template specialization for sf::Music
template<typename ID>
class ResourceManager<sf::Music, ID>
{
public:

	ResourceManager(const ResourceManager&) = delete; // no copy

	ResourceManager& operator=(const ResourceManager&) = delete; // no copy

	ResourceManager() = default;

	template<typename... Args>
	void Load(const ID& id, Args&& ... args);

	void Unload(const ID& id);

	void Clear();

	sf::Music& Get(const ID& id)const;


	void LoadFromJson(const std::string& jsonPath);

private:
	std::unordered_map<ID, std::unique_ptr<sf::Music>> m_resources;

};


template<typename ID>
template<typename ...Args>
inline void ResourceManager<sf::Music, ID>::Load(const ID& id, Args && ...args)
{
	std::unique_ptr<sf::Music> ptr = std::make_unique<sf::Music>();// (new sf::Music);

	if (!ptr->openFromFile(std::forward<Args>(args)...))
	{
		throw std::runtime_error("Error loading resource");
	}

	if (m_resources.emplace(id, std::move(ptr)).second == false)
	{
		throw std::runtime_error("Error inserting resource, is already load?");
	}

	std::cout << "Resource loaded: " << id << std::endl;
}

template<typename ID>
inline sf::Music& ResourceManager<sf::Music, ID>::Get(const ID& id) const
{
	auto it = m_resources.find(id);
	if (it == m_resources.end())
	{
		throw std::runtime_error("Resource with ID not found");
	}
	return *(it->second);
}

template<typename ID>
inline void ResourceManager<sf::Music, ID>::Unload(const ID& id)
{
	auto it = m_resources.find(id);
	if (it == m_resources.end())
	{
		throw std::runtime_error("Resource with ID not found");
	}
	m_resources.erase(it);
	std::cout << "Resource unloaded: " << id << std::endl;
}

template<typename ID>
inline void ResourceManager<sf::Music, ID>::Clear()
{
	m_resources.clear();
	std::cout << "All resources cleared" << std::endl;
}



template <typename ID>
inline void ResourceManager<sf::Music, ID>::LoadFromJson(const std::string& jsonPath)
{
	std::ifstream file(jsonPath);

	if (!file.is_open())
	{
		throw std::runtime_error("Error opening file: " + jsonPath);
	}

	nlohmann::json jsonData;
	file >> jsonData;

	for (auto& resource : jsonData)
	{
		std::string src = resource["src"];
		std::string id = resource["id"];
		std::string type = StringUtil::ToLower(resource["type"]);

		if (type == "music")
		{
			Load(id, src);
		}
	}

}