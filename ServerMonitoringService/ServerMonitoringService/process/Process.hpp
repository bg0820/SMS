#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <Windows.h>
#include <Psapi.h>
#include <tlhelp32.h>
#include "Winternl.h"
#include <time.h>

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
	tm createTime = { 0 };
private:
	HWND getHwndFromPid();
	HANDLE getHandleFromPid();
	HICON initIcon(BOOLEAN LargeIcon = FALSE);
	TCHAR* initName();
	TCHAR* initPath();
	TCHAR* initCommandLine();
	TCHAR* initOwner();
	tm initCreateTime();
public:
	Process(const DWORD pid = 0) : pid(pid)
	{
		// fixed value Init
		this->handle = getHandleFromPid();
		this->hWnd = getHwndFromPid();
		
		strcpy_s(this->owner, initOwner());
		strcpy_s(this->name, initName());
		strcpy_s(this->path, initPath());
		this->commandLine = initCommandLine();

		this->icon = initIcon(TRUE);
		this->createTime = initCreateTime();
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

		if (icon)
			DestroyIcon(icon);			
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