#include "DataManager.hpp"

int main()
{
	DataManager dataManager;
	dataManager.Update();
}

void DataManager::Update()
{
	// ProcessList Update
	processListObj.Update();
	system->processCount = processListObj.getCount();

	// Process Update
	system->process = new Process[system->processCount];
	for (int i = 0; i < system->processCount; i++)
	{
		int pid = processListObj.getPID(i);
		system->process[i] = processListObj.getProcess(pid);
	}

	// Cpu Usage Update
	systemCpu.getUsage(system->cpuUsageVal);
	systemCpu.getIdleUsage(system->cpuIdleVal);

	// Disk Usage Update
	system->disk = systemDiskIO.getDiskList(system->diskCount);

	// Memory Update
	 systemMemory.getFreeByte(system->memoryFreeByte);
	 systemMemory.getUsedByte(system->memoryUsedByte);
	 systemMemory.getTotalByte(system->memoryTotalByte);
	 systemMemory.getLoadPercent(system->memoryLoadPercent);

	 // Network Update
	 systemNetwork.getConnectionTable(system->networkConnection, system->networkConnectionCount);
	 systemNetwork.getInterfaces(system->adapterInfo, system->adapterCount);


}
