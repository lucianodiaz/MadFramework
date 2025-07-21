#pragma once
#include <functional>

template<typename T>
class Tween
{
public:
	// Constructor for Tween with a reference to the input value

	Tween(T* InputValue, T start, T end, float duration, std::function<float(float)> easingFunc)
		: m_inputValue(InputValue), m_startValue(start), m_endValue(end), m_duration(duration), m_easingFunc(easingFunc), m_id(0) { }

private:
	T* m_inputValue; // Reference to the input value that will be updated
	T m_startValue;
	T m_endValue;
	float m_duration;
	float m_elapsedTime = 0.0f;
	std::function<float(float)> m_easingFunc;

	std::uint32_t m_id;
public:
	void update(float deltaTime);
};

template<typename T>
inline void Tween<T>::update(float deltaTime)
{
	if (m_elapsedTime < m_duration)
	{
		m_elapsedTime += deltaTime;
		float t = m_elapsedTime / m_duration;
		float easedValue = m_easingFunc(t);
		T currentValue = m_startValue + (m_endValue - m_startValue) * easedValue;

		if (m_inputValue) {
			*m_inputValue = currentValue; // Update the value through the pointer
		}
	}
	else
	{
		if (m_inputValue) {
			*m_inputValue = m_endValue; // Ensure the final value is set
		}
		
	}
}
