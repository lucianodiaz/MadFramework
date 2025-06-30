#include <Gameplay/AnimationController.h>
#include <Core/World.h>

void AnimationController::Play(const std::string& animationName, bool loop)
{
	if (m_animationComponent.animations.find(animationName) != m_animationComponent.animations.end())
	{

		if (m_animationComponent.currentAnimation != animationName)
		{
			m_animationComponent.currentAnimation = animationName;
			auto& animationData = m_animationComponent.animations[animationName];
			animationData.isLooping = loop;
			animationData.isPlaying = true;
			animationData.isPaused = false;
			animationData.elapsedTime = 0.0f;
			animationData.currentFrame = 0;

			m_animationComponent.texture = animationData.texture;
			m_animationComponent.sprite.setTexture(m_animationComponent.texture);
		}

	}
}

void AnimationController::Pause()
{
	if (m_animationComponent.animations.find(m_animationComponent.currentAnimation) != m_animationComponent.animations.end())
	{
		auto& animationData = m_animationComponent.animations[m_animationComponent.currentAnimation];
		if (animationData.isPlaying)
		{
			animationData.isPaused = true;
			animationData.isPlaying = false; // Stop the animation when paused
		}
	}
}

void AnimationController::Resume()
{
	if (m_animationComponent.animations.find(m_animationComponent.currentAnimation) != m_animationComponent.animations.end())
	{
		auto& animationData = m_animationComponent.animations[m_animationComponent.currentAnimation];
		if (animationData.isPaused)
		{
			animationData.isPaused = false;
			animationData.isPlaying = true; // Resume the animation
		}
	}
}

void AnimationController::Stop()
{
	if (m_animationComponent.animations.find(m_animationComponent.currentAnimation) != m_animationComponent.animations.end())
	{
		auto& animationData = m_animationComponent.animations[m_animationComponent.currentAnimation];
		animationData.isPlaying = false;
		animationData.isPaused = false;
		animationData.elapsedTime = 0.0f;
		animationData.currentFrame = 0;
	}
}

void AnimationController::LoadAnimation(const nlohmann::json& animationData)
{


	auto& nameAnimationData = animationData["name"];
	auto& srcFilePathData = animationData["src"];

	std::string nameAnimation;
	std::string srcFilePath;
	for (auto& name : nameAnimationData.items())
	{
		nameAnimation = name.value();
		// Assuming World::GetWorld()->LoadTexture is a method to load textures
		// World::GetWorld()->LoadTexture(name, nameAnimationData);
	}
	for (auto& src : srcFilePathData.items())
	{

		srcFilePath = src.value();
		// Load the texture from the file path
		// Assuming World::GetWorld()->LoadTexture is a method to load textures
		 World::GetWorld()->LoadTexture(nameAnimation, srcFilePath);
	}

	//World::GetWorld()->LoadTexture(nameAnimation, srcFilePath);

	auto& animations = animationData["animations"];

	for (auto& [name, anim] : animations.items())
	{
		AnimationData data;

		data.name = nameAnimation;
		data.frameRect.left = anim["startX"];
		data.frameRect.top = anim["startY"];
		data.frameRect.width = anim["frameWidth"];
		data.frameRect.height = anim["frameHeight"];
		data.spriteSizeX = anim["spriteSizeX"];
		data.spriteSizeY = anim["spriteSizeY"];
		data.totalFrames = anim["frameCount"];
		data.frameDuration = anim["frameDuration"];
		data.isLooping = anim["loop"];

		
		data.texture = World::GetWorld()->GetTexture(data.name);
		data.sprite.setTexture(data.texture);

		m_animationComponent.animations[name] = data;
	}
}
