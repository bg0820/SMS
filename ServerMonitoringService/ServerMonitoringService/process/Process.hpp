#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <Windows.h>
#include <Psapi.h>
#include <tlhelp32.h>
#include <iostream>
#include <Winuser.h>
#include "Winternl.h"

#pragma comment(lib, "psapi.lib")

#define GCL_HICON (-14)

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
	TCHAR commandLine[MAX_PATH];
private:
	HANDLE handleFromPid();
	HWND hWndFromPid();
	// TODO initFunc retrun Type TCHAR* > int, initName(TCHAR *val)
	TCHAR* initName();
	TCHAR* initPath();
	TCHAR* initCommandLine();
	PVOID GetPebAddress(HANDLE processHandle);
	ULONG procIdFromHwnd(HWND hwnd);
	HICON initIcon(BOOLEAN LargeIcon = FALSE);
public:
	Process(const DWORD pid = 0)
	{
		// fixed value Init
		Process::pid = pid;
		handle = handleFromPid();
		hWnd = hWndFromPid();
		strcpy_s(name, initName());
		strcpy_s(path, initPath());
		strcpy_s(commandLine, initCommandLine());
		icon = initIcon(TRUE);
	}

	~Process()
	{
		cout << "call ~Process()" << endl;

		if (handle != INVALID_HANDLE_VALUE)
			CloseHandle(handle);

		if (icon)
			DestroyIcon(icon);
	}

	DWORD getPid();
	TCHAR* getName();
	TCHAR* getPath();
	TCHAR* getCommandLine();
	HICON getIcon();

	int getHandleCount(DWORD &val);
	int getThreadCount(int &parmTotalThreadCount, int &parmCurrentProcessThreadCount);
	int getNetworkUsage(double &val);
	int getMemoryUsage(DWORD &val);
	int getCpuUsage(double &val);
};
#endif