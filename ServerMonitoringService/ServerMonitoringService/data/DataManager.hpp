#ifndef _DATAMANAGER_H_
#define _DATAMANAGER_H_

#include <Windows.h>
#include "../process/ProcessList.hpp"
#include "../process/Process.hpp"


typedef struct system
{
	double cpuVal;
	double cpuIdleVal;
	DWORD memoryUseVal;
	DWORD memoryFreeVal;
	Process *process;
}System;


class DataManager
{
private:
	DWORD *processes = NULL;
	System system;
	ProcessList processListObj = ProcessList();
public:
	DataManager()
	{

	}

	~DataManager()
	{

	}

	void Update();
};

#endif