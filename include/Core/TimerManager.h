#pragma once
#include <SFML/System.hpp>
#include <map>
#include <functional>
#include <set>

class TimerManager
{
public:
	TimerManager() = default;


	int CreateTimer(float duration, std::function<void()> callback, bool loop = false);

	bool IsTimerExpired(int timerID);

	void RemoveTimer(int timerID);

	void Update();

private:
	std::set<int> _timersToRemove;

	struct TimerInfo {
		sf::Clock clock;
		sf::Time duration;
		bool loop;
		std::function<void()> callback;
	};

	int nextTimerID = 0;
	std::map<int, TimerInfo> timers;
};
