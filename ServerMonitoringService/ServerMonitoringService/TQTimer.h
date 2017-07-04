#ifndef _TQTIMER_H_
#define _TQTIMER_H_

#include <Windows.h>
#include <functional>
using namespace std;

#define FAILED_CREATE_TIMER -10

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
	function<void()> func = NULL;
public:
	TQTimer(function<void()> parmFunc)
	{
		timerCount++;
		timerID = timerCount - 1;
		func = parmFunc;
	}

	~TQTimer()
	{
		if (timerQueue)
			Stop();
	}

	void Start();
	void Stop();
	UINT getInterval();
	void setInterval(const UINT interval);
};

#endif // !_TQTIMERE_H_