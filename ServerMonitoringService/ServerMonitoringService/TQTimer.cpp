#include "TQTimer.h"

UINT TQTimer::timerCount = 0;

void CALLBACK TQTimer::TimerProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	TQTimer* obj = reinterpret_cast<TQTimer*>(lpParameter);
	if (obj->func)
		obj->func();
}

void TQTimer::Start()
{
	if (TQTimer::timerQueue)
		Stop();

	TQTimer::timerQueue = CreateTimerQueue();

	if (CreateTimerQueueTimer(&(TQTimer::timerHandle), TQTimer::timerQueue, TQTimer::TimerProc, reinterpret_cast<PVOID>(this), 0, (DWORD)TQTimer::interval, WT_EXECUTEINTIMERTHREAD) == 0)
		throw FAILED_CREATE_TIMER;

}

void TQTimer::Stop()
{
	DeleteTimerQueueTimer(TQTimer::timerQueue, TQTimer::timerHandle, NULL);
}

UINT TQTimer::getInterval()
{
	return TQTimer::interval;
}

void TQTimer::setInterval(const UINT interval)
{
	TQTimer::interval = interval;
}
