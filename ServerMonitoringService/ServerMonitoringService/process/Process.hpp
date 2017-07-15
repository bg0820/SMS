#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <shlobj.h>
#include <shlguid.h>
#include <shellapi.h>
#include <commctrl.h>
#include <commoncontrols.h>

#include <Windows.h>
#include <Psapi.h>
#include <Winternl.h>
#include <time.h>
#include "../util/StopWatch.hpp"

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
	HANDLE handle = NULL;
	// HWND hWnd = NULL; // Call only when needed
	TCHAR *commandLine = NULL;
	TCHAR *name = NULL;
	TCHAR *path = NULL;
	TCHAR *owner = NULL;
	tm createTime = { 0 };
private:
	HWND getHwndFromPid();
	HANDLE getHandleFromPid();
	void initFileNamePath();
	void initOwner();
	void initCommandLine();
	void initCreateTime();
public:
	Process(const DWORD pid = 0) : pid(pid)
	{
		// fixed value Init
		this->handle = getHandleFromPid(); // 0.005 ~ 0.01ms
		// this->hWnd = getHwndFromPid(); // 0.2 ~ 3.0 ms Call only when needed
		initOwner();
		initFileNamePath();
		initCommandLine();
		initCreateTime();
		//this->icon = getIcon(TRUE); // cpu time 81.4% usage
	}

	~Process()
	{
		if (this->handle != INVALID_HANDLE_VALUE)
			CloseHandle(this->handle);

		if (this->name)
		{
			delete[] this->name;
			this->name = nullptr;
		}

		if (this->path)
		{
			delete[] this->path;
			this->path = nullptr;
		}

		if (this->owner)
		{
			delete[] this->owner;
			this->owner = nullptr;
		}

		if (this->commandLine)
		{
			delete[] this->commandLine;
			this->commandLine = nullptr;
		}

		/*
			if (this->icon)
				DestroyIcon(this->icon);
		*/
	}

	BOOLEAN isRunning();
	DWORD getPid();
	TCHAR* getName();
	TCHAR* getPath();
	TCHAR* getCommandLine();
	TCHAR* getOwner();
	HICON getIcon(BOOLEAN LargeIcon);
	int getHandleCount(DWORD &val);
	int getNetworkUsage(double &val);
	int getMemoryUsage(DWORD &val);
	int getCpuUsage(double &val);
};
#endif