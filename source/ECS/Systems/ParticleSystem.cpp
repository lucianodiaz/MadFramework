#include <ECS/Systems/ParticleSystem.h>
#include <ECS/Components/TransformComponent.h>
#include <ECS/ECSManager.h>
#include <Core/World.h>

ParticleSystem::ParticleSystem(std::unique_ptr<ECSManager>& ecs) : m_ecs(ecs)
{
}


static void integrate(ParticleEmitterComponent& e, float dt)
{
	for (int i = int(e.alive.size()) - 1; i >= 0; --i)
	{
		auto& p = e.pool[e.alive[i]];
		p.age += dt;
		if (p.age >= p.lifetime)
		{
			e.killIndex(i);
			continue;
		}

		p.vel += e.settings.gravity * dt;
		if (e.settings.drag > 0.0f)
		{
			p.vel *= (1.0f - e.settings.drag * dt);
		}

		p.pos += p.vel * dt;
		p.rot += p.rotSpeed * dt;
	}
}

void ParticleSystem::UpdateEntities(float dt)
{
	auto entities = m_ecs->GetEntitiesWithComponent<ParticleEmitterComponent, TransformComponent>();

	for (auto e : entities)
	{
		const auto& tr = m_ecs->GetComponent<TransformComponent>(e);
		for (auto* em : m_ecs->GetComponents<ParticleEmitterComponent>(e))
		{
			if (!em->enabled) continue;

			bool canEmit = em->playback.playing;

			if (canEmit && em->playback.startDelay > 0.0f) {
				em->playback.startDelay -= dt;
				if (em->playback.startDelay > 0.0f) { integrate(*em, dt); continue; }
			}

			if (canEmit && em->playback.duration >= 0.0f) {
				em->playback.elapsed += dt;
				if (em->playback.elapsed >= em->playback.duration) {
					canEmit = false;
					em->playback.playing = (em->playback.mode == EmitterPlayMode::Loop);
					if (em->playback.playing) em->playback.elapsed = 0.0f;
				}
			}

			if (canEmit) {
				if (em->settings.burst) {
					for (std::uint32_t i = 0; i < em->settings.burstCount; ++i) em->SpawnOne(tr.position);
					em->settings.burst = false;
					if (em->playback.mode == EmitterPlayMode::Once && em->playback.duration < 0.0f) {
						em->playback.playing = false;
					}
				}
				else if (em->settings.spawnRate > 0.0f) {
					em->spawnAccumulator += dt * em->settings.spawnRate;
					while (em->spawnAccumulator >= 1.0f) { em->spawnAccumulator -= 1.0f; em->SpawnOne(tr.position); }
				}
			}

			integrate(*em, dt);

			if (em->playback.mode == EmitterPlayMode::Once &&
				em->playback.duration < 0.0f && em->playback.stopWhenEmpty && em->alive.empty())
			{
				em->enabled = false;
				em->playback.playing = false;
			}
		}
	}
}



void ParticleSystem::Render(sf::RenderWindow& window)
{
	auto entities = m_ecs->GetEntitiesWithComponent<ParticleEmitterComponent, TransformComponent>();
	for (auto e : entities)
	{
		for (auto* em : m_ecs->GetComponents<ParticleEmitterComponent>(e))
		{
			if (em->alive.empty()) continue;

			sf::RenderStates states;

			sf::Texture* texPtr = nullptr;
			if (!em->settings.textureId.empty())
			{
				texPtr = &World::GetWorld()->GetTexture(em->settings.textureId);
				states.texture = texPtr;
				sf::VertexArray va(sf::Quads);
				va.resize(em->alive.size() * 4);
				std::size_t v = 0;

				// Determina el rect de textura a usar
				sf::IntRect r = em->settings.texRect;
				if (r.width == 0 || r.height == 0)
					r = { 0, 0, (int)texPtr->getSize().x, (int)texPtr->getSize().y };

				for (auto idx : em->alive)
				{
					const auto& p = em->pool[idx];

					// Calcula half-extents con aspecto si se desea
					float halfH = p.size * 0.5f;
					float aspect = static_cast<float>(r.width) / static_cast<float>(r.height == 0 ? 1 : r.height);
					float halfW = em->settings.sizeFromTexture ? (halfH * aspect) : halfH;

					// Rotación en torno a p.pos
					float c = std::cos(p.rot), s = std::sin(p.rot);
					auto rot = [&](sf::Vector2f local)->sf::Vector2f {
						return { local.x * c - local.y * s, local.x * s + local.y * c };
						};

					sf::Vector2f p0 = p.pos + rot({ -halfW, -halfH });
					sf::Vector2f p1 = p.pos + rot({ +halfW, -halfH });
					sf::Vector2f p2 = p.pos + rot({ +halfW, +halfH });
					sf::Vector2f p3 = p.pos + rot({ -halfW, +halfH });

					va[v + 0].position = p0; va[v + 1].position = p1; va[v + 2].position = p2; va[v + 3].position = p3;

					// UVs
					va[v + 0].texCoords = { static_cast<float>(r.left),               static_cast<float>(r.top) };
					va[v + 1].texCoords = { static_cast<float>(r.left + r.width),     static_cast<float>(r.top) };
					va[v + 2].texCoords = { static_cast<float>(r.left + r.width),     static_cast<float>(r.top + r.height) };
					va[v + 3].texCoords = { static_cast<float>(r.left),               static_cast<float>(r.top + r.height) };

					// Color por vértice (tinte + alpha por vida si alguna vez lo quieres)
					va[v + 0].color = va[v + 1].color = va[v + 2].color = va[v + 3].color = p.color;

					v += 4;
				}

				sf::RenderStates states;
				states.texture = texPtr;
				states.blendMode = em->settings.useAdditive ? sf::BlendAdd : sf::BlendAlpha;
				window.draw(va, states);
				continue; // ya renderizamos este emisor; sigue con el siguiente
			}

			sf::Shader* shaderPtr = nullptr;
			if (!em->settings.shaderId.empty()) {
				shaderPtr = &World::GetWorld()->GetShader(em->settings.shaderId);
				// Set uniforms comunes (por emisor/frame)
				shaderPtr->setUniform("u_time", em->playback.elapsed * em->settings.u_time_scale);
				shaderPtr->setUniform("u_color", em->settings.u_tint);
				shaderPtr->setUniform("u_glowColor", sf::Glsl::Vec3(0.f, 1.f, 0.f));
				if (texPtr) shaderPtr->setUniform("u_texture", *texPtr);
				states.shader = shaderPtr;
			}

			if (em->settings.asQuads) {
				sf::VertexArray va(sf::Quads);
				va.resize(em->alive.size() * 4);
				std::size_t v = 0;
				for (auto idx : em->alive) {
					const auto& p = em->pool[idx];
					float hs = p.size * 0.5f;
					va[v + 0].position = p.pos + sf::Vector2f(-hs, -hs);
					va[v + 1].position = p.pos + sf::Vector2f(+hs, -hs);
					va[v + 2].position = p.pos + sf::Vector2f(+hs, +hs);
					va[v + 3].position = p.pos + sf::Vector2f(-hs, +hs);
					va[v + 0].color = va[v + 1].color = va[v + 2].color = va[v + 3].color = p.color;
					v += 4;
				}
				window.draw(va,states);
			}
			else {
				sf::VertexArray points(sf::Points);
				points.resize(em->alive.size());
				for (std::size_t i = 0; i < em->alive.size(); ++i) {
					const auto& p = em->pool[em->alive[i]];
					points[i].position = p.pos; points[i].color = p.color;
				}
				window.draw(points, states);
			}
		}
	}
}