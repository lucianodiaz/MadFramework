#pragma once  
#include <functional>  
#include <memory>
#include <list>  
#include <Animation/Tween.h>  

class TweenManager  
{  
public:  
	TweenManager() = default;  
	~TweenManager() = default;  
	// Add methods for managing tweens, such as adding, updating, and removing tweens  

	template<typename T>  
	std::uint32_t CreateTween(T* OutputValue,T StartValue, T endValue, float duration, std::function<float(float)> easingFunc = MAD::Easings::Linear, std::function<void(void)> onComplete=nullptr);  
	void Update(float deltaTime);  
	
	template<typename T>
	T& GetTween(std::uint32_t id);

private:
	void RemoveTween(std::uint32_t id);
	std::list<std::unique_ptr<ITween>> m_tweensToAdd;
	std::list<std::unique_ptr<ITween>> m_tweens;

};

template<typename T>
inline std::uint32_t TweenManager::CreateTween(T* OutputValue, T StartValue, T endValue, float duration, std::function<float(float)> easingFunc, std::function<void(void)> onComplete)
{
	std::unique_ptr<Tween<T>> tween = std::make_unique<Tween<T>>(OutputValue, StartValue, endValue, duration, easingFunc,m_tweens.size(),onComplete);
	std::uint32_t tweenId = tween->GetId();
	m_tweensToAdd.emplace_back(std::move(tween));
	return tweenId;
}

template<typename T>
inline T& TweenManager::GetTween(std::uint32_t id)
{
	// TODO: Insertar una instrucción "return" aquí
	for (const auto& tween : m_tweens)
	{
		if (tween->getId() == id)
		{
			return static_cast<T&>(*tween);
		}
	}
}
