#include "TQTimer.hpp"

UINT TQTimer::timerCount = 0;

void CALLBACK TQTimer::TimerProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	// copy
	TQTimer* obj = (TQTimer*)lpParameter;

	if (obj->func)
		obj->func();
}

void TQTimer::Start()
{
	if (TQTimer::timerQueue) // timer is exist
		Stop();

	TQTimer::timerQueue = CreateTimerQueue();

	// cast reinterpret_cast<PVOID>(this);
	if (CreateTimerQueueTimer(&(TQTimer::timerHandle), TQTimer::timerQueue, TQTimer::TimerProc, (PVOID)this, 0, (DWORD)TQTimer::interval, WT_EXECUTEINTIMERTHREAD) == 0)
		throw FAILED_CREATE_TIMER;

}

void TQTimer::Stop()
{
	// DeleteTimerQueueTimer function does CloseHandle() timerQueue, timerHandle That You don't need to call a CloseHandle() function
	if (TQTimer::timerQueue || TQTimer::timerHandle)
		DeleteTimerQueueTimer(TQTimer::timerQueue, TQTimer::timerHandle, NULL);

	TQTimer::timerQueue = nullptr;
	TQTimer::timerHandle = nullptr;
}

UINT TQTimer::getInterval()
{
	return TQTimer::interval;
}

void TQTimer::setInterval(const UINT interval)
{
	TQTimer::interval = interval;
}
