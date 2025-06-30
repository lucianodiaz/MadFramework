#pragma once
#include <SFML/System.hpp>
#include <map>
#include <functional>
#include <set>

class TimerManager
{
public:
	TimerManager() = default;


	int createTimer(float duration, std::function<void()> callback, bool loop = false);

	bool isTimerExpired(int timerID);

	void removeTimer(int timerID);

	void update();

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
