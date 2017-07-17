#ifndef _THREAD_H_
#define _THREAD_H_

#include <functional>
#include <Windows.h>
#include <thread>

using namespace std;

class Thread
{
private:
	static UINT threadCount;
	HANDLE threadHandle = NULL;
	//HANDLE timerQueue = NULL;
	UINT threadID = 0;
	LPVOID lpParam;
	//UINT interval = 0;
	static DWORD WINAPI Thread::threadProc(LPVOID lpParameter);
public:
	function<DWORD(LPVOID)> func = NULL;
public:
	Thread(function<DWORD(LPVOID)> paramFunc, LPVOID lpParam)
	{
		this->threadCount++;
		this->func = paramFunc;
		this->lpParam = lpParam;
	}

	~Thread()
	{
		if (threadHandle)
			Abort();
	}

	void Create();
	void Abort();
};

#endif // !_THREAD_H_