#include <ECS/Systems/ParticleSystem.h>
#include <ECS/Components/TransformComponent.h>


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

void ParticleSystem::UpdateEntities(float deltaTime)
{
	auto entities = m_ecs->GetEntitiesWithComponent<ParticleEmitterComponent, TransformComponent>();

	for (auto entity : entities)
	{
		auto& em = m_ecs->GetComponent<ParticleEmitterComponent>(entity);
		if (!em.enabled) continue;

		//spawn
		const auto& tr = m_ecs->GetComponent<TransformComponent>(entity);

		if (em.settings.burst)
		{
			for (std::uint32_t i = 0; i < em.settings.burstCount; ++i)
			{
				em.SpawnOne(tr.position);
				em.settings.burst = false;
			}
		}
		else if (em.settings.spawnRate > 0.0f)
		{
			em.spawnAccumulator += deltaTime * em.settings.spawnRate;
			while (em.spawnAccumulator >= 1.0f)
			{
				em.spawnAccumulator -= 1.0f;
				em.SpawnOne(tr.position);
			}
		}

		integrate(em, deltaTime);
	}

}


void ParticleSystem::Draw(sf::RenderWindow& window)
{

	auto entities = m_ecs->GetEntitiesWithComponent<ParticleEmitterComponent, TransformComponent>();

	for (auto entity : entities) {
		auto& em = m_ecs->GetComponent<ParticleEmitterComponent>(entity);
		if (em.alive.empty()) continue;

		if (em.settings.asQuads) {
			sf::VertexArray va(sf::Quads);
			va.resize(em.alive.size() * 4);

			std::size_t v = 0;
			for (auto idx : em.alive) {
				const auto& p = em.pool[idx];
				float hs = p.size * 0.5f;

				// (sin texturas: quads coloreados; con textura -> añade texCoords)
				va[v + 0].position = p.pos + sf::Vector2f(-hs, -hs);
				va[v + 1].position = p.pos + sf::Vector2f(+hs, -hs);
				va[v + 2].position = p.pos + sf::Vector2f(+hs, +hs);
				va[v + 3].position = p.pos + sf::Vector2f(-hs, +hs);
				va[v + 0].color = va[v + 1].color = va[v + 2].color = va[v + 3].color = p.color;
				v += 4;
			}
			window.draw(va);
		}
		else {
			sf::VertexArray points(sf::Points);
			points.resize(em.alive.size());
			for (std::size_t i = 0; i < em.alive.size(); ++i) {
				const auto& p = em.pool[em.alive[i]];
				points[i].position = p.pos;
				points[i].color = p.color;
			}
			window.draw(points);
		}
	}
}