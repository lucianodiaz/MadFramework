#include <Core/TimerManager.h>


/**
 * Create a Timer that work with seconds
 * @duration ammount of time you set this timer
 * @callback method that you call when this timer finished
 * @loop true this timer called indefinitly
 * @return a ID of this timer
 */
int TimerManager::CreateTimer(float duration, std::function<void()> callback, bool loop)
{
    int timerID = nextTimerID++;

    timers[timerID] = { sf::Clock(),sf::seconds(duration),loop,callback };

    return timerID;
}

bool TimerManager::IsTimerExpired(int timerID)
{
    if (timers.find(timerID) != timers.end())
    {
        return timers[timerID].clock.getElapsedTime() >= timers[timerID].duration;
    }
    return false;
}

void TimerManager::RemoveTimer(int timerID)
{
    _timersToRemove.insert(timerID);
}

void TimerManager::Update() {
    //std::set<int> timersToRemove;

    for (auto it = timers.begin(); it != timers.end(); ++it) {
        int timerID = it->first;
        TimerInfo& timerInfo = it->second;

        if (IsTimerExpired(timerID)) {
            if (timerInfo.callback) {
                timerInfo.callback();
            }

            if (timerInfo.loop) {
                timerInfo.clock.restart();
            }
        }
    }

    for (auto it = timers.begin(); it != timers.end(); ++it) {
        int timerID = it->first;
        TimerInfo& timerInfo = it->second;

        if (IsTimerExpired(timerID) && !timerInfo.loop) {
            _timersToRemove.insert(timerID);
        }
    }

    for (int timerID : _timersToRemove) {
        timers.erase(timerID);
    }
    _timersToRemove.clear();
}