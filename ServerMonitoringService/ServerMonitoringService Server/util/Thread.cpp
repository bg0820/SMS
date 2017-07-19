#include "Thread.hpp"

DWORD WINAPI ThreadWrapper::threadProc(LPVOID lpParameter)
{
	// copy
	ThreadWrapper* obj = (ThreadWrapper*)lpParameter;

	if (obj->func)
		obj->func(obj->lpParam);

	return 1;
}

void ThreadWrapper::run()
{
	if (this->threadHandle) // timer is exist
		abort();

	ULONG threadID;
	this->threadHandle= CreateThread(NULL, 0, threadProc, (PVOID)this, 0, &threadID);
	this->threadID = threadID;
}

void ThreadWrapper::abort()
{
	DWORD result = 0;
	GetExitCodeThread(this->threadHandle, &result);

	if (threadHandle)
		CloseHandle(threadHandle);
}
