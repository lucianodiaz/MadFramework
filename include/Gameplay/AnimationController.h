#pragma once
#include <ECS/Components/SpriteAnimationComponent.h>
#include <nlohmann/json.hpp>
#include <MadFrameworkExport.h>
class MAD_API AnimationController
{
public:
	AnimationController(SpriteAnimationComponent& animationComponent)
		: m_animationComponent(animationComponent) {}

	void Play(const std::string& animationName, bool loop = true);

	void Pause();

	void Resume();

	void Stop();

	bool isPlaying() const { return m_animationComponent.animations[m_animationComponent.currentAnimation].isLooping; };

	std::string GetCurrentAnimation() const { return m_animationComponent.currentAnimation; }

	void LoadAnimation(const nlohmann::json& animationData);

private:
	SpriteAnimationComponent& m_animationComponent; // Reference to the animation component being controlled
};