#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <Windows.h>
#include <Psapi.h>
#include <tlhelp32.h>
#include "Winternl.h"

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
	HICON icon;
	HANDLE handle = NULL;
	HWND hWnd = NULL;
	TCHAR name[MAX_PATH];
	TCHAR path[MAX_PATH];
	TCHAR owner[MAX_PATH];
	TCHAR *commandLine = NULL;
private:
	HWND getHwndFromPid();
	HANDLE getHandleFromPid();
	HICON initIcon(BOOLEAN LargeIcon = FALSE);
	TCHAR* initName();
	TCHAR* initPath();
	TCHAR* initCommandLine();
	TCHAR* initOwner();
public:
	Process(const DWORD pid = 0) : pid(pid)
	{
		// fixed value Init
		this->handle = getHandleFromPid();
		this->hWnd = getHwndFromPid();
		
		strcpy_s(owner, initOwner());
		strcpy_s(name, initName());
		strcpy_s(path, initPath());
		commandLine = initCommandLine();

		icon = initIcon(TRUE);
	}

	~Process()
	{
		if (handle != INVALID_HANDLE_VALUE)
			CloseHandle(handle);

		if (commandLine)
		{
			delete[] commandLine;
			commandLine = nullptr;
		}
	}

	BOOLEAN isRunning();
	DWORD getPid();
	TCHAR* getName();
	TCHAR* getPath();
	TCHAR* getCommandLine();
	TCHAR* getOwner();
	HICON getIcon();

	int getHandleCount(DWORD &val);
	int getThreadCount(int &paramTotalThreadCount, int &paramCurrentProcessThreadCount);
	int getNetworkUsage(double &val);
	int getMemoryUsage(DWORD &val);
	int getCpuUsage(double &val);
};
#endif