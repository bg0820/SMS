#include "Thread.hpp"

DWORD WINAPI Thread::threadProc(LPVOID lpParameter)
{
	// copy
	Thread* obj = (Thread*)lpParameter;

	if (obj->func)
		obj->func(obj->lpParam);

	return 1;
}

void Thread::Create()
{
	if (this->threadHandle) // timer is exist
		Abort();

	ULONG threadID;
	this->threadHandle= CreateThread(NULL, 0, threadProc, (PVOID)this, 0, &threadID);
	this->threadID = threadID;
}

void Thread::Abort()
{
	DWORD result = 0;
	GetExitCodeThread(this->threadHandle, &result);

	if (threadHandle)
		CloseHandle(threadHandle);
}
