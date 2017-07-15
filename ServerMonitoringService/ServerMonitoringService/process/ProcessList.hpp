#ifndef _PROCESSLIST_H_
#define _PROCESSLIST_H_

#include <iostream>
#include <Windows.h>
#include <Psapi.h>
#include <tlhelp32.h>
#include <unordered_map>

#pragma comment(lib, "psapi.lib")

using namespace std;

typedef unsigned long       DWORD;

class ProcessList
{
private:
	int processListCount = 128; // += 128
	DWORD *processList;
	DWORD processListSize = NULL;
	unordered_map<int, int> threadCounting;
private:
	int getThreadCount();
	BOOLEAN isContainsKey(int paramPID);
public:
	ProcessList()
	{
		processList = new DWORD[processListCount];
		getThreadCount();
	}

	~ProcessList()
	{
		if (processList)
		{
			delete[] processList;
			processList = nullptr;
		}
	}
	void Update(); // process list update
	int getCount(); // update -> return value is processList count
	int getPID(const int i); // return value is num i to process PID value
	int getProcessThreadCount(const int paramPID);

};

#endif