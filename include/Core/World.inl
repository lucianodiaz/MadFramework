#pragma once

template<typename T>
inline std::shared_ptr<T>& World::CreateGUI()
{
	static_assert(std::is_base_of<UserWidget, T>::value, "T must derive from UserWidget");

	std::shared_ptr<T> userWidget = std::make_shared<T>();
	m_sceneManager.AddUserWidget(userWidget);
	return userWidget;
}

template<typename T, typename ...Args>
inline T& World::SpawnActor(Args && ...args)
{
	static_assert(std::is_base_of<Actor, T>::value, "T must derive from Actor");

	m_actors.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
	m_actors.back()->Start();
	return static_cast<T&>(*m_actors.back());
}

template<typename T>
inline std::shared_ptr<T> World::GetSystem()
{
	for (auto& system : m_sceneManager.GetECSManager()->GetSystems())
	{
		if (auto castedSystem = std::dynamic_pointer_cast<T>(system))
		{
			return castedSystem;
		}
	}

	return nullptr;
}
