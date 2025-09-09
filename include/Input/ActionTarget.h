#pragma once
#include <MadFrameworkExport.h>
#include <functional>
#include <list>
#include <utility>
#include "ActionMap.h"


enum class MAD_API FireMode {OnPressed, OnReleased, WhileDown};
enum class MAD_API FireSource {Poll, RealTime };


template<typename T = int>
class ActionTarget
{
public:
	ActionTarget(const ActionTarget<T>&) = delete;
	ActionTarget<T>& operator=(const ActionTarget<T>&) = delete;

	using FuncType = std::function<void(const sf::Event&)>;

	ActionTarget(const ActionMap<T>& map);

	void processEvent(const sf::Event& event)const;
	void processEvents()const;

	void bind(const T& key, FireMode mode, FireSource source, const FuncType& cb);
	void bindPressed(const T& key, const FuncType& cb, FireSource src = FireSource::Poll)	{ bind(key, FireMode::OnPressed, src, cb); }
	void bindReleased(const T& key, const FuncType& cb, FireSource src = FireSource::Poll)	{ bind(key, FireMode::OnReleased, src, cb); }
	void bindWhileDown(const T& key, const FuncType& cb)									{ bind(key, FireMode::WhileDown, FireSource::RealTime, cb); }


	void unbind(const T& key);
protected:
	ActionTarget(ActionMap<T>& map) : _actionMap(map) {}

	//const ActionMap<T>& _actionMap;
private:

	struct Binding {
		T key;
		FuncType cb;
		FireMode mode;
		FireSource source;
		mutable bool wasDown = false; //  for flanks in real-time processing
	};

	ActionMap<T>& _actionMap;

	std::vector<Binding> _pollBindings;
	std::vector<Binding> _realtimeBindings;

};

template<typename T>
inline ActionTarget<T>::ActionTarget(const ActionMap<T>& map) :
	_actionMap(map)
{
}

template<typename T>
inline void ActionTarget<T>::processEvent(const sf::Event& event) const 
{
	if (_pollBindings.empty()) return;

	for (const Binding& b : _pollBindings) 
	{
		const Action& action = _actionMap.get(b.key);

		if (!(action == event)) continue;

		if (b.mode == FireMode::OnPressed && (event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed)) 
		{
			b.cb(event);
		}
		else if (b.mode == FireMode::OnReleased && (event.type == sf::Event::KeyReleased || event.type == sf::Event::MouseButtonReleased)) 
		{
			b.cb(event);
		}
	}
}

template<typename T>
inline void ActionTarget<T>::processEvents() const 
{
	if (_realtimeBindings.empty()) return;

	for (Binding& b : const_cast<std::vector<Binding>&>(_realtimeBindings)) 
	{
		const Action& action = _actionMap.get(b.key);
		const bool nowDown = action.isDown();

		switch (b.mode)
		{
			case FireMode::OnPressed:
				if (nowDown && !b.wasDown) b.cb(action._event);
				break;
			case FireMode::OnReleased:
				if (!nowDown && b.wasDown) b.cb(action._event);
				break;
			case FireMode::WhileDown:
				if (nowDown) b.cb(action._event);
				break;
		}
		b.wasDown = nowDown;
	}
}

template<typename T>
inline void ActionTarget<T>::bind(const T& key, FireMode mode, FireSource source, const FuncType& cb) 
{
	Binding b{ key, cb, mode, source, false };

	if (source == FireSource::RealTime) 
	{
		_realtimeBindings.push_back(std::move(b));
	}
	else {
		_pollBindings.push_back(std::move(b));
	}
}

template<typename T>
inline void ActionTarget<T>::unbind(const T& key) {
	auto rem = [&](auto& vec) {
		vec.erase(std::remove_if(vec.begin(), vec.end(),
			[&](const Binding& b) { return b.key == key; }),
			vec.end());
		};
	rem(_pollBindings);
	rem(_realtimeBindings);
}