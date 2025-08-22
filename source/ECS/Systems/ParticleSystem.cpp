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

            // 1) Un único states por emisor
            sf::RenderStates states;
            states.blendMode = em->settings.useAdditive ? sf::BlendAdd : sf::BlendAlpha;

            // texture (opcional)
            if (em->settings.texPtr)
                states.texture = em->settings.texPtr;

            // shader (opcional)
            if (em->settings.shaderPtr)
            {
                states.shader = em->settings.shaderPtr;

                // Si el shader espera una textura con nombre distinto a "texture"
                // em->settings.shaderPtr->setUniform("u_texture", sf::Shader::CurrentTexture);

                // Uniforms propios (si los usás en el shader)
                // em->settings.shaderPtr->setUniform("u_time", m_clock.getElapsedTime().asSeconds() * em->settings.u_time_scale);
                // em->settings.shaderPtr->setUniform("u_tint", em->settings.u_tint);
            }

            // 2) Construcción de geometría
            if (em->settings.texPtr) // QUADS con UVs
            {
                sf::VertexArray va(sf::Quads);
                va.resize(em->alive.size() * 4);
                std::size_t v = 0;

                sf::IntRect r = em->settings.texRect;
                if (r.width == 0 || r.height == 0)
                    r = { 0, 0, (int)em->settings.texPtr->getSize().x, (int)em->settings.texPtr->getSize().y };

                for (auto idx : em->alive)
                {
                    const auto& p = em->pool[idx];

                    float halfH = p.size * 0.5f;
                    float aspect = (float)r.width / (float)(r.height == 0 ? 1 : r.height);
                    float halfW = em->settings.sizeFromTexture ? (halfH * aspect) : halfH;

                    float c = std::cos(p.rot), s = std::sin(p.rot);
                    auto rot = [&](sf::Vector2f L) { return sf::Vector2f(L.x * c - L.y * s, L.x * s + L.y * c); };

                    sf::Vector2f p0 = p.pos + rot({ -halfW, -halfH });
                    sf::Vector2f p1 = p.pos + rot({ +halfW, -halfH });
                    sf::Vector2f p2 = p.pos + rot({ +halfW, +halfH });
                    sf::Vector2f p3 = p.pos + rot({ -halfW, +halfH });

                    va[v + 0].position = p0; va[v + 1].position = p1; va[v + 2].position = p2; va[v + 3].position = p3;

                    va[v + 0].texCoords = { (float)r.left,             (float)r.top };
                    va[v + 1].texCoords = { (float)(r.left + r.width),   (float)r.top };
                    va[v + 2].texCoords = { (float)(r.left + r.width),   (float)(r.top + r.height) };
                    va[v + 3].texCoords = { (float)r.left,             (float)(r.top + r.height) };

                    va[v + 0].color = va[v + 1].color = va[v + 2].color = va[v + 3].color = p.color;
                    v += 4;
                }

                window.draw(va, states); // usa states con shader (si hay)
            }
            else if (em->settings.asQuads) // QUADS sin textura (shader que NO samplee)
            {
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
                window.draw(va, states);
            }
            else // POINTS
            {
                sf::VertexArray points(sf::Points);
                points.resize(em->alive.size());
                for (std::size_t i = 0; i < em->alive.size(); ++i) {
                    const auto& p = em->pool[em->alive[i]];
                    points[i].position = p.pos;
                    points[i].color = p.color;
                }
                window.draw(points, states);
            }
        }
    }
}