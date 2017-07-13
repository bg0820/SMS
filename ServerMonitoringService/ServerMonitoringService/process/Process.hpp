#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <Windows.h>
#include <Psapi.h>
#include <tlhelp32.h>
#include <iostream>

#pragma comment(lib, "psapi.lib")

using namespace std;

typedef unsigned long       DWORD;
typedef NTSTATUS(NTAPI *_NtQueryInformationProcess)(
	HANDLE ProcessHandle,
	DWORD ProcessInformationClass,
	PVOID ProcessInformation,
	DWORD ProcessInformationLength,
	PDWORD ReturnLength
	);

int Compare(const void * val1, const void * val2);

class Process
{
private:
	DWORD pid;
	HANDLE handle;
	TCHAR name[MAX_PATH];
	TCHAR path[MAX_PATH];
	TCHAR *commandLine;
private:
	HANDLE getHandleFromPID();
	// TODO initFunc retrun Type TCHAR* > int, initName(TCHAR *val)
	TCHAR* initName();
	TCHAR* initPath();
	TCHAR* initCommandLine();
	PVOID GetPebAddress(HANDLE ProcessHandle);
public:
	Process(const DWORD pid = 0) : pid(pid)
	{
		if (pid != 0)
		{
			// fixed value Init
			Process::handle = getHandleFromPID();

			// strcpy_s(Multi-Byte) -> wcscpy_s(UNICODE)
			strcpy_s(name, initName());
			strcpy_s(path, initPath());
			// commadLine add
			commandLine = initCommandLine();
		}
	}

	~Process()
	{
		if (pid != 0)
		{
			if (handle != INVALID_HANDLE_VALUE)
				CloseHandle(handle);

			if (commandLine)
			{
				delete[] commandLine;
				commandLine = nullptr;
			}
		}
	}

	DWORD getPid();
	TCHAR* getName();
	TCHAR* getPath();
	TCHAR* getCommandLine();

	int getHandleCount(DWORD &val);
	int getThreadCount(int &paramTotalThreadCount, int &paramCurrentProcessThreadCount);
	int getNetworkUsage(double &val);
	int getMemoryUsage(DWORD &val);
	int getCpuUsage(double &val);
};
#endif