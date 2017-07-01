#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <Windows.h>
#include <Psapi.h>

#pragma comment(lib, "psapi.lib")

typedef unsigned long       DWORD;

int Compare(const void * val1, const void * val2);

class Process
{
public :
	DWORD pid;
public:
	Process(const DWORD pid = 0)
	{
		Process::pid = pid;
	}

	DWORD getPid();
	TCHAR* getName();
	TCHAR* getPath();
	TCHAR* getCommandLine();
	int getNetworkUsage(double &val);
	int getMemoryUsage(DWORD &val);
	int getCpuUsage(double &val);
};
#endif