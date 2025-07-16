#include <Transitions/FadeTransition.h>
#include <Core/World.h>

FadeTransition::FadeTransition(Fade fade, float duration) : m_fade(fade), m_duration(duration), m_elapsed(0.0f)
{
	m_isFinished = false;
}

void FadeTransition::OnStart()
{
	std::cout << "On Start FadeTransition " << (m_fade == Fade::In ? "In" : "Out") << std::endl;

	m_overlay.setSize(sf::Vector2f(World::GetWorld()->GetWindow().GetRenderWindow().getSize()));

	m_overlay.setFillColor(sf::Color(0, 0, 0, (m_fade== Fade::In) ? 255 : 0));
}

void FadeTransition::Update(float deltaTime)
{
	m_elapsed += deltaTime;

	float progress = std::min(m_elapsed / m_duration, 1.0f);

	m_alpha = (m_fade == Fade::In) ? (1.0f - progress) * 255 : progress * 255;

	if (progress >= 1.0f && !m_isFinished)
	{
		m_isFinished = true;
	}
}

void FadeTransition::Draw(sf::RenderWindow& window)
{
	m_overlay.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_alpha)));
	window.draw(m_overlay);
}

bool FadeTransition::IsFinished() const
{
	return m_isFinished;
}

void FadeTransition::OnEnd()
{
	std::cout << "On End FadeTransition "<< (m_fade == Fade::In ? "In" : "Out") << std::endl;
	m_isFinished = false;
	//m_overlay.setFillColor(sf::Color::Black);
	
}
