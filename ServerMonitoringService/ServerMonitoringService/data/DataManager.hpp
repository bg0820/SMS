#ifndef _DATAMANAGER_H_
#define _DATAMANAGER_H_

#include <Windows.h>
#include "../process/ProcessList.hpp"
#include "../process/Process.hpp"
#include "../system/SystemCPU.hpp"
#include "../system/SystemDiskIO.hpp"
#include "../system/SystemMemory.hpp"
#include "../system/SystemNetwork.hpp"
#include "../system/SystemOS.hpp"
#include "../util/TQTimer.hpp"

typedef struct system
{
	double cpuUsageVal;
	double cpuIdleVal;
	string cpuModel;

	DWORD memoryLoadPercent;
	DWORDLONG memoryUsedByte;
	DWORDLONG memoryFreeByte;
	DWORDLONG memoryTotalByte;

	Disk *disk = NULL;

	Process *process = NULL;
}System;


class DataManager
{
private:
	DWORD *processes = NULL;
	ProcessList processListObj = ProcessList();

	System *system;
	SystemCpu systemCpu;
	SystemDiskIO systemDiskIO;
	SystemMemory systemMemory;
	SystemNetwork systemNetwork;
	SystemOS systemOS;
public:
	DataManager()
	{
		system = new System;
		// fixed variable
		system->cpuModel = systemCpu.getCpuModeInfo();

		// Start Function Call
		systemDiskIO.Start();
	}

	~DataManager()
	{
		if (system)
		{
			if (system->disk)
			{
				delete[] system->disk;
				system->disk = nullptr;
			}
			
			if (system->process)
			{
				delete[] system->process;
				system->process = nullptr;
			}

			delete system;
			system = nullptr;
		}
	}

	void Update();
};

#endif