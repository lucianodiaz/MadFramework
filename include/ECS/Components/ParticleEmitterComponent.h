#pragma once
#include <ECS/Component.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include "ParticlesTypes.h"
#include <MathUtils.h>

struct ParticleEmitterComponent : public IComponent
{
	EmitterSettings settings{};

	std::vector<ParticleInstance> pool;
	std::vector<std::uint32_t> freeList;
	std::vector<std::uint32_t> alive;
	float spawnAccumulator = 0.0f;
	bool enabled = true;

	sf::Vector2f localOffset{ 0.0f,0.0f };

	ParticleEmitterComponent(const EmitterSettings& s = {}) : settings(s)
	{
		Reserve(s.maxParticles);
	}

	void Reserve(std::uint32_t n)
	{
		settings.maxParticles = n;
		pool.assign(n, {});
		freeList.clear();
		freeList.reserve(n);
		alive.clear();
		alive.reserve(n);

		for (std::uint32_t i = 0; i < n; ++i)
		{
			freeList.emplace_back(i);
		}
	}

	bool SpawnOne(const sf::Vector2f& worldPos)
	{
		if (freeList.empty()) return false;

		auto index = freeList.back();
		freeList.pop_back();
		auto& p = pool[index];

		p.active = true;
		p.age = 0.0f;
		p.lifetime = MAD::MathUtils::frand(settings.lifetimeMin, settings.lifetimeMax);
		p.size = MAD::MathUtils::frand(settings.sizeMin, settings.sizeMax);
		p.rot = MAD::MathUtils::frand(settings.rotMin, settings.rotMax);
		p.rotSpeed = MAD::MathUtils::frand(settings.rotSpeedMin, settings.rotSpeedMax);
		p.vel = MAD::MathUtils::vrand(settings.velMin, settings.velMax);
		p.color = settings.color;
		p.pos = worldPos + localOffset;
		alive.push_back(index);
		return true;
	}

	void killIndex(std::uint32_t idxAlive)
	{
		auto idx = alive[idxAlive];
		pool[idx].active = false;
		alive[idxAlive] = alive.back();
		alive.pop_back();
		freeList.push_back(idx);
	}
};