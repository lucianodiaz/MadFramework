#pragma once
#include <ECS/Component.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include "ParticlesTypes.h"
#include <MathUtils.h>
#include <Core/World.h>

enum class EmitterPlayMode { Loop, Once };

struct EmitterPlayback
{
	bool playing = true;
	EmitterPlayMode mode = EmitterPlayMode::Loop;
	float startDelay = 0.0f;
	float duration = -1.0f; //emmiting time; <0 = infinite
	float elapsed = 0.0f;
	bool stopWhenEmpty = true; // in Mode Once, Stops when theres not any particle live
};

struct ParticleEmitterComponent : public IComponent
{
	EmitterSettings settings{};
	EmitterPlayback playback{};

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
		if (!settings.textureId.empty()) {
			const auto& tex = World::GetWorld()->GetTexture(settings.textureId);
			sf::Vector2u full = tex.getSize();
			sf::Vector2u sz = (settings.texRect.width > 0 && settings.texRect.height > 0)
				? sf::Vector2u(settings.texRect.width, settings.texRect.height)
				: full;
			float base = MAD::MathUtils::frand(settings.sizeMin, settings.sizeMax);
			// usa 'base' como alto y ajusta ancho por aspecto:
			float aspect = static_cast<float>(sz.x) / static_cast<float>(sz.y == 0 ? 1 : sz.y);
			p.size = base; // interpretaremos size como "alto" y el ancho = alto*aspect
		}
		else
		{
			p.size = MAD::MathUtils::frand(settings.sizeMin, settings.sizeMax);
		}
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

	void PlayLoop(float delaySec = 0.0f)
	{
		enabled = true;
		playback = {};
		playback.playing = true;
		playback.mode = EmitterPlayMode::Loop;
		playback.startDelay = delaySec;
	}

	void PlayOnce(float durationSec = -1.0f, float delaySec = 0.0f, bool stopWhenEmpty_ = true)
	{
		enabled = true;
		playback = {};
		playback.playing = true;
		playback.mode = EmitterPlayMode::Once;
		playback.duration = durationSec;
		playback.startDelay = delaySec;
		playback.stopWhenEmpty = stopWhenEmpty_;
	}

	void Stop()
	{
		playback.playing = false;
		spawnAccumulator = 0.0f;
	}

	void StopAfter(float seconds)
	{
		if (seconds <= 0) { Stop(); return; }

		playback.playing = true;
		playback.mode = EmitterPlayMode::Once;
		playback.duration = seconds;
		playback.elapsed = 0.0f;
	}

	void Pause() { playback.playing = false; }
	void Resume() { playback.playing = true; }

	void KillAll()
	{
		for (auto idxAlive : alive) { pool[idxAlive].active = false; freeList.push_back(idxAlive); }
		alive.clear();
		spawnAccumulator = 0.0f;
	}
};