#include <ECS/Systems/AnimationSystem.h>
#include <ECS/ECSManager.h>

AnimationSystem::AnimationSystem(std::unique_ptr<ECSManager>& ecs) : m_ecs(ecs) 
{
}

void AnimationSystem::UpdateEntities(float deltaTime)
{

    for (auto e : m_ecs->GetEntitiesWithComponent<SpriteAnimationComponent>())
    {
        for (auto* anim : m_ecs->GetComponents<SpriteAnimationComponent>(e))
        {
            if (anim->animations.empty()) continue;
            auto& data = anim->animations[anim->currentAnimation];
            if (!data.isPlaying || data.isPaused) continue;

            data.elapsedTime += deltaTime;
            if (data.elapsedTime >= data.frameDuration)
            {
                data.elapsedTime = 0.0f;
                data.currentFrame++;
                if (data.currentFrame >= data.totalFrames)
                {
                    if (data.isLooping) data.currentFrame = 0;
                    else { data.currentFrame = data.totalFrames - 1; data.isPlaying = false; }
                }
                data.frameRect.left = data.frameRect.width * data.currentFrame;
                anim->sprite.setTextureRect(data.frameRect);
            }
        }
    }
}
