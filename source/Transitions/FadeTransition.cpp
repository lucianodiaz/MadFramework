#include <Transitions/FadeTransition.h>
#include <iostream>
#include <Core/World.h>
#include <Window/Window.h>


FadeTransition::FadeTransition(FadeType type, float duration, sf::Color color)
    : m_type(type), m_duration(duration), m_color(color), m_currentTime(0.0f), m_finished(false)
{
}

void FadeTransition::OnStart()
{
    m_currentTime = 0.0f;
    m_finished = false;

    // Set up overlay to cover entire screen
    // Note: This should be set based on window size, not hardcoded
    auto windowSize = World::GetWorld()->GetWindow().GetSize(); // You should get this from World/Window
    m_overlay.setSize(sf::Vector2f(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)));
    m_overlay.setPosition(0, 0);

    // Set initial alpha based on fade type
    sf::Uint8 initialAlpha = (m_type == FadeOut) ? 0 : 255;
    m_color.a = initialAlpha;
    m_overlay.setFillColor(m_color);

    std::cout << "[FadeTransition] Started " << (m_type == FadeOut ? "FadeOut" : "FadeIn")
        << " transition for " << m_duration << " seconds" << std::endl;
}

void FadeTransition::Update(float deltaTime)
{
    if (m_finished) return;

    m_currentTime += deltaTime;

    // Calculate alpha based on progress and fade type
    float progress = std::min(m_currentTime / m_duration, 1.0f);

    sf::Uint8 alpha;
    if (m_type == FadeOut) {
        // Fade out: go from 0 (transparent) to 255 (opaque)
        alpha = static_cast<sf::Uint8>(progress * 255);
    }
    else {
        // Fade in: go from 255 (opaque) to 0 (transparent)
        alpha = static_cast<sf::Uint8>((1.0f - progress) * 255);
    }

    m_color.a = alpha;
    m_overlay.setFillColor(m_color);

    // Check if finished
    if (progress >= 1.0f) {
        m_finished = true;
        std::cout << "[FadeTransition] Finished " << (m_type == FadeOut ? "FadeOut" : "FadeIn") << std::endl;
    }
}

void FadeTransition::Draw(sf::RenderWindow& window)
{
    // Update overlay size to match current window size
    auto windowSize = window.getSize();
    m_overlay.setSize(sf::Vector2f(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)));

    window.draw(m_overlay);
}

bool FadeTransition::IsFinished() const
{
    return m_finished;
}

void FadeTransition::OnEnd()
{
    std::cout << "[FadeTransition] OnEnd called" << std::endl;
    m_finished = true;
}