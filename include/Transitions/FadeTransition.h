#pragma once
#include <Core/ISceneTransition.h>

class FadeTransition : public ISceneTransition
{
public:
    enum FadeType {
        FadeOut,  // Fade from transparent to opaque (covers scene)
        FadeIn    // Fade from opaque to transparent (reveals scene)
    };

    FadeTransition(FadeType type, float duration, sf::Color color = sf::Color::Black);

    void OnStart() override;
    void Update(float deltaTime) override;
    void Draw(sf::RenderWindow& window) override;
    bool IsFinished() const override;
    void OnEnd() override;

private:
    FadeType m_type;
    float m_duration;
    float m_currentTime;
    sf::Color m_color;
    bool m_finished;
};