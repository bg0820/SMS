#ifndef _THREAD_H_
#define _THREAD_H_

#include <functional>
#include <Windows.h>
#include <thread>

using namespace std;

class ThreadWrapper
{
private:
	static UINT threadCount;
	HANDLE threadHandle = NULL;
	//HANDLE timerQueue = NULL;
	UINT threadID = 0;
	LPVOID lpParam;
	//UINT interval = 0;
	static DWORD WINAPI threadProc(LPVOID lpParameter);
public:
	function<void(LPVOID)> func = NULL;
public:
	ThreadWrapper(function<void(LPVOID)> paramFunc, HANDLE lpParam)
	{
		this->threadCount++;
		this->func = paramFunc;
		this->lpParam = lpParam;
	}

	~ThreadWrapper()
	{
		if (threadHandle)
			abort();
	}

	void run();
	void abort();
};

#endif // !_THREAD_H_