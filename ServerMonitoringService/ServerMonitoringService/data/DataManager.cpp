#include "DataManager.hpp"

int main()
{
	DataManager dataManager;
	dataManager.
}

void DataManager::Update()
{
	// ProcessList Update
	processListObj.Update();
	systemInfo->processCount = processListObj.getCount();

	// Process Update
	systemInfo->process = new Process[systemInfo->processCount];
	for (int i = 0; i < systemInfo->processCount; i++)
	{
		int pid = processListObj.getPID(i);
		systemInfo->process[i] = Process(pid);
	}

	// Cpu Usage Update
	systemCpu.getUsage(systemInfo->cpuUsageVal);
	systemCpu.getIdleUsage(systemInfo->cpuIdleVal);

	// Disk Usage Update
	systemDiskIO.Update();
	systemInfo->disk = systemDiskIO.getDiskList(systemInfo->diskCount);

	// Memory Update
	 systemMemory.getFreeByte(systemInfo->memoryFreeByte);
	 systemMemory.getUsedByte(systemInfo->memoryUsedByte);
	 systemMemory.getTotalByte(systemInfo->memoryTotalByte);
	 systemMemory.getLoadPercent(systemInfo->memoryLoadPercent);

	 // Network Update
	 systemNetwork.getConnectionTable(systemInfo->networkConnection, systemInfo->networkConnectionCount);
	 systemNetwork.getInterfaces(systemInfo->adapterInfo, systemInfo->adapterCount);
}


void DataManager::CallbackProc()
{
	// 1Sec Update
	Update();
}

void DataManager::Start()
{
	if (this->tqTimer == NULL)
	{
		this->tqTimer = new TQTimer(std::bind(&DataManager::CallbackProc, this));
		this->tqTimer->setInterval(1000); // 1Sec
		this->tqTimer->Start();
	}
}

void DataManager::Stop()
{
	if (this->tqTimer)
	{
		this->tqTimer->Stop();

		delete this->tqTimer;
		this->tqTimer = nullptr;
	}
}

SystemInfo* DataManager::getSystem()
{
	return this->systemInfo;
}
