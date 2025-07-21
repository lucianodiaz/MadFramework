#include <Core/TweenManager.h>


void TweenManager::Update(float deltaTime)
{
	//Remove finished tweens before updating
	if(m_tweensToAdd.size () > 0)
	{
		for (auto& tween : m_tweensToAdd)
		{
			m_tweens.emplace_back(std::move(tween));
		}
	
	}
	m_tweensToAdd.clear();

	m_tweens.remove_if([](const std::unique_ptr<ITween>& tween) {
		return tween->isFinished();
		});

	for(auto & tween : m_tweens)
	{
		tween->update(deltaTime);
	}

}

void TweenManager::RemoveTween(std::uint32_t id)
{
	for (auto it = m_tweens.begin(); it != m_tweens.end(); ++it)
	{
		if ((*it)->GetId() == id)
		{
			m_tweens.erase(it);
			break;
		}
	}
}
