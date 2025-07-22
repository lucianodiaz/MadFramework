#pragma once
#include <functional>

class ITween {
public:
	virtual ~ITween() = default;
	virtual void update(float deltaTime) = 0;
	virtual bool isFinished() const = 0;
	virtual void Pause() = 0;
	virtual void Resume() = 0;
	virtual std::uint32_t GetId() const = 0;
	std::function<void(void)> m_onCompleteTween;

protected:
	bool m_isLooping = false; // Flag to indicate if the tween should loop
	bool m_isPaused = false; // Flag to indicate if the tween is paused
	bool m_yoyo = false; // Flag to indicate if the tween should yoyo (reverse direction on completion)
};

template<typename T>
class Tween : public ITween
{
public:
	// Constructor for Tween with a reference to the input value

	Tween(T* InputValue, T start, T end, float duration, std::function<float(float)> easingFunc, std::uint32_t id, std::function<void(void)> onCompleteTween = nullptr)
		: m_inputValue(InputValue), m_startValue(start), m_endValue(end), m_duration(duration), m_easingFunc(easingFunc), m_id(id) 
	{
		m_onCompleteTween = onCompleteTween;
	}

private:
	T* m_inputValue; // Reference to the input value that will be updated
	T m_startValue;
	T m_endValue;
	float m_duration;
	float m_elapsedTime = 0.0f;
	std::function<float(float)> m_easingFunc;
	
	std::uint32_t m_id;
public:
	void update(float deltaTime) override;

	bool isFinished()  const override { return m_elapsedTime >= m_duration; }

	std::uint32_t GetId() const override { return m_id; }
	void Pause() override { m_isPaused = true; }
	void Resume() override { m_isPaused = false; }

};

template<typename T>
inline void Tween<T>::update(float deltaTime)
{
	if (m_isPaused) return;

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

		if(m_onCompleteTween)
		{
			m_onCompleteTween(); // Call the completion callback if provided
		}
		
	}
}
