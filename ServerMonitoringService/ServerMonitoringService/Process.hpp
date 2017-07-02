#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <Windows.h>
#include <Psapi.h>
#include <iostream>
using namespace std;
#pragma comment(lib, "psapi.lib")

typedef unsigned long       DWORD;

int Compare(const void * val1, const void * val2);

class Process
{
private:
	DWORD pid;
	HANDLE handle;
private:
	int getHandleFromPID();
public:
	Process(const DWORD pid = 0)
	{
		Process::pid = pid;
		getHandleFromPID();
	}

	~Process()
	{
		if (handle)
			CloseHandle(handle);
	}

	DWORD getPid();
	TCHAR* getName();
	TCHAR* getPath();
	TCHAR* getCommandLine();
	int getHandleCount(int &val);
	int getThreadCount(int &val);
	int getNetworkUsage(double &val);
	int getMemoryUsage(DWORD &val);
	int getCpuUsage(double &val);
};
#endif