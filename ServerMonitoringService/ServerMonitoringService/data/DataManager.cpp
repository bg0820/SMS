#include "DataManager.hpp"

int main()
{
	DataManager dataManager;
	dataManager.Update();

//	dataManage
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
		systemInfo->process[i] = processListObj.getProcess(pid);
	}

	// Cpu Usage Update
	systemCpu.getUsage(systemInfo->cpuUsageVal);
	systemCpu.getIdleUsage(systemInfo->cpuIdleVal);

	// Disk Usage Update
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

SystemInfo* DataManager::getSystem()
{
	return this->systemInfo;
}
