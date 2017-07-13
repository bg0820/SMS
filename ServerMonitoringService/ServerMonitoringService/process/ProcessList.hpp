#ifndef _PROCESSLIST_H_
#define _PROCESSLIST_H_

#include <iostream>
#include <Windows.h>
#include <Psapi.h>
#include <vector>
#include "Process.hpp"

#pragma comment(lib, "psapi.lib")

using namespace std;

typedef unsigned long       DWORD;

class ProcessList
{
private:
	int processListCount = 0;
	DWORD *processList;
	DWORD processListSize = NULL;
public:
	ProcessList()
	{
		//processList.reserve(128);
		processListCount = 128;
		processList = new DWORD[processListCount];
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
	Process* getProcess(const DWORD processId); // return value is processId index to Process Obj
	int getPID(const int i); // return value is num i to process PID value
							 // TODO: function Add
							 // getProcessUser, getProcessStatus, getProcessNetworkSend, 
							 // getProcessNetworkReceive, getProcessDescription
};

#endif