#pragma once
#include <Core/ISceneTransition.h>

enum class Fade
{
	Out,
	In
};
class FadeTransition : public ISceneTransition
{
public:



	FadeTransition(Fade fade,float duration);

	void OnStart() override;
	void Update(float deltaTime) override;
	void Draw(sf::RenderWindow& window) override;
	bool IsFinished() const override;
	void OnEnd() override;

private:
	bool m_isFinished;
	float m_alpha{};
	Fade m_fade;
	float m_duration;
	float m_elapsed;
};