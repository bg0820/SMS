#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <Windows.h>
#include <Psapi.h>
#include <tlhelp32.h>
#include <iostream>

#pragma comment(lib, "psapi.lib")

using namespace std;

typedef unsigned long       DWORD;

int Compare(const void * val1, const void * val2);

class Process
{
private:
	DWORD pid;
	HANDLE handle;
	TCHAR name[MAX_PATH];
	TCHAR path[MAX_PATH];
	TCHAR commandLine[MAX_PATH];
private:
	int getHandleFromPID();
	TCHAR* initName();
	TCHAR*  initPath();
	TCHAR*  initCommandLine();
public:
	Process(const DWORD pid = 0)
	{
		// fixed value Init
		Process::pid = pid;
		getHandleFromPID();

		// strcpy_s(Multi-Byte) -> wcscpy_s(UNICODE)
		wcscpy_s(name, initName());
		wcscpy_s(path, initPath());
		wcscpy_s(commandLine, initCommandLine());
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

	int getHandleCount(DWORD &val);
	int getThreadCount(int &val);
	int getNetworkUsage(double &val);
	int getMemoryUsage(DWORD &val);
	int getCpuUsage(double &val);
};
#endif