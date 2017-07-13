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

typedef struct system_info
{
	double cpuUsageVal;
	double cpuIdleVal;
	string cpuModel;

	DWORD memoryLoadPercent;
	DWORDLONG memoryUsedByte;
	DWORDLONG memoryFreeByte;
	DWORDLONG memoryTotalByte;

	string userName;
	string computerName;
	string osVersion;

	Disk *disk = NULL;
	int diskCount = 0;
	Process *process = NULL;
	int processCount = 0;
	NetworkConnection *networkConnection = NULL;
	ULONG networkConnectionCount;

	PIP_ADAPTER_INFO adapterInfo;
	int adapterCount = 0;
}SystemInfo;

class DataManager
{
private:
	DWORD *processes = NULL;
	ProcessList processListObj = ProcessList();

	SystemInfo *systemInfo;
	SystemCpu systemCpu;
	SystemDiskIO systemDiskIO;
	SystemMemory systemMemory;
	SystemNetwork systemNetwork;
	SystemOS systemOS;

	TQTimer *tqTimer;
private:
	void Update();
	void Start();
	void Stop();
	void CallbackProc();
public:
	DataManager()
	{
		systemInfo = new SystemInfo;
		// fixed variable
		systemInfo->cpuModel = systemCpu.getCpuModeInfo();
		systemInfo->computerName = systemOS.getComputerName();
		systemInfo->userName = systemOS.getUserName();
		systemInfo->osVersion = systemOS.getOSVersionName();

		// first Call
		this->Update();

		// Start Function Call
		systemDiskIO.Start();
		this->Start();
	}

	~DataManager()
	{
		if (systemInfo)
		{
			if (systemInfo->disk)
			{
				delete[] systemInfo->disk;
				systemInfo->disk = nullptr;
			}
			
			if (systemInfo->process)
			{
				delete[] systemInfo->process;
				systemInfo->process = nullptr;
			}

			if (systemInfo->process)
			{
				delete[] systemInfo->networkConnection;
				systemInfo->networkConnection = nullptr;
			}

			delete systemInfo;
			systemInfo = nullptr;
		}

		systemDiskIO.Stop();
		this->Stop();
	}

	SystemInfo* getSystem();
};

#endif