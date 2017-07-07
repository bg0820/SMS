#ifndef _TQTIMER_H_
#define _TQTIMER_H_

#include <functional>
#include <Windows.h>

#define FAILED_CREATE_TIMER -10

using namespace std;

class TQTimer
{
private:
	static UINT timerCount;
	HANDLE timerHandle = NULL;
	HANDLE timerQueue = NULL;
	UINT timerID = 0;
	UINT interval = 0;
	static void CALLBACK TimerProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
public:
	function<void(void)> func = NULL;
public:
	TQTimer(function<void(void)> paramFunc)
	{
		timerCount++;
		timerID = timerCount - 1;
		func = paramFunc;
	}

	~TQTimer()
	{
		if (timerQueue || timerHandle)
			Stop();
	}

	void Start();
	void Stop();
	UINT getInterval();
	void setInterval(const UINT interval);
};

#endif // !_TQTIMERE_H_