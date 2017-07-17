#ifndef _DATAMANAGER_H_
#define _DATAMANAGER_H_

#include <iostream>
#include <Windows.h>
#include "../process/ProcessList.hpp"
#include "../process/Process.hpp"
#include "../system/SystemCPU.hpp"
#include "../system/SystemDiskIO.hpp"
#include "../system/SystemMemory.hpp"
#include "../system/SystemNetwork.hpp"
#include "../system/SystemOS.hpp"
#include "../util/TQTimer.hpp"
#include "../util/StopWatch.hpp"
#include "../rapidjson/document.h"
#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/writer.h"

using namespace rapidjson;
using namespace std;

typedef struct system_info
{
	double cpuUsageVal;
	double cpuIdleVal;
	TCHAR cpuModel[1024];

	int memoryLoadPercent;
	ULONGLONG memoryUsedByte;
	ULONGLONG memoryFreeByte;
	ULONGLONG memoryTotalByte;

	TCHAR userName[MAX_PATH];
	TCHAR computerName[MAX_PATH];
	TCHAR osVersion[MAX_PATH];

	Disk *disk = NULL;
	int diskCount = 0;
	Process **process = NULL;
	int processCount = 0;
	NetworkConnection *networkConnection = NULL;
	ULONG networkConnectionCount;

	IP_ADAPTER_INFO *adapterInfo = NULL;
	int adapterCount = 0;
}SystemInfo;

class DataManager
{
private:
	SystemInfo *systemInfo = NULL;
	TQTimer *tqTimer = NULL;

	ProcessList processListObj = ProcessList();
	SystemCpu systemCpu;
	SystemDiskIO systemDiskIO;
	SystemMemory systemMemory;
	SystemNetwork systemNetwork;
	SystemOS systemOS;
private:
	void Update();
	Value getJsonDiskList(Document::AllocatorType &alloc);
	Value getJsonProcessList(Document::AllocatorType &alloc);
	void jsonUpdate();
	void Start();
	void Stop();
	void CallbackProc();
public:
	DataManager()
	{
		systemInfo = new SystemInfo;
		// fixed variable
		strcpy_s(systemInfo->cpuModel, systemCpu.getCpuModeInfo());
		strcpy_s(systemInfo->computerName, systemOS.getComputerName());
		strcpy_s(systemInfo->userName, systemOS.getUserName());
		strcpy_s(systemInfo->osVersion, systemOS.getOSVersionName());

		// first Call
		this->Update();

		// Start Function Call
		systemDiskIO.Start();
		this->Start();
	}

	~DataManager()
	{
		systemDiskIO.Stop();
		this->Stop();

		if (systemInfo)
		{
			/*
				~SystemDiskIO()
				delete[] systemInfo->disk;
			*/

			if (systemInfo->process)
			{
				for (int i = 0; i < systemInfo->processCount; i++)
				{
					if (systemInfo->process[i])
					{
						delete systemInfo->process[i];
						systemInfo->process[i] = nullptr;
					}
				}

				delete[] systemInfo->process;
				systemInfo->process = nullptr;
			}

			if (systemInfo->networkConnection)
			{
				delete[] systemInfo->networkConnection;
				systemInfo->networkConnection = nullptr;
			}

			if (systemInfo->adapterInfo)
			{
				delete[] systemInfo->adapterInfo;
				systemInfo->adapterInfo = nullptr;
			}

			if (systemInfo->networkConnection)
			{
				delete[] systemInfo->networkConnection;
				systemInfo->networkConnection = nullptr;
			}

			delete systemInfo;
			systemInfo = nullptr;
		}
	}

	SystemInfo* getSystem();
};

#endif