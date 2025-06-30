#include <ECS/Systems/AnimationSystem.h>

void AnimationSystem::UpdateEntities(float deltaTime)
{

	auto entites = m_ecs->GetEntitiesWithComponent<SpriteAnimationComponent>();

	for (auto& entity : entites)
	{
		auto& animation = m_ecs->GetComponent<SpriteAnimationComponent>(entity);

		if (animation.animations.empty()) 			continue;

		auto& data = animation.animations[animation.currentAnimation];

		if (!data.isPlaying || data.isPaused) continue;

		data.elapsedTime += deltaTime;

		if (data.elapsedTime >= data.frameDuration)
		{
			data.elapsedTime = 0.0f; // Reset elapsed time
			data.currentFrame++;

			if (data.currentFrame >= data.totalFrames)
			{
				if (data.isLooping)
				{
					data.currentFrame = 0;
				}
				else
				{
					data.currentFrame = data.totalFrames - 1; // Stop at the last frame
					data.isPlaying = false; // Stop the animation
				}
			}
			data.frameRect.left = data.frameRect.width * data.currentFrame;
			animation.sprite.setTextureRect(data.frameRect);
		}
	}
}
