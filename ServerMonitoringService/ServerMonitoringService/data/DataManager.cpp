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
	int processCount = processListObj.getCount();

	// Process Update
	system->process = new Process[processCount];
	for (int i = 0; i < processCount; i++)
	{
		int pid = processListObj.getPID(i);
		system->process[i] = processListObj.getProcess(pid);
	}

	// Cpu Usage Update
	systemCpu.getUsage(system->cpuUsageVal);
	systemCpu.getIdleUsage(system->cpuIdleVal);

	// Disk Usage Update
	int count = 0;
	system->disk = systemDiskIO.getDiskList(count);
	
	/*for (int i = 0; i < count; i++)
	{
	cout << "Read/sec : " << disk[i].readSec / 1024.0 << "kb/s" << endl;
	cout << "Write/sec : " << disk[i].writeSec / 1024.0 << "kb/s" << endl;
	}*/

	// Memory Update
	 systemMemory.getFreeByte(system->memoryFreeByte);
	 systemMemory.getUsedByte(system->memoryUsedByte);
	 systemMemory.getTotalByte(system->memoryTotalByte);
	 systemMemory.getLoadPercent(system->memoryLoadPercent);

	 systemNetwork.getConnectionTable(system->networkConnection, system->networkConnectionCount);

	 for (ULONG i = 0; i < count; i++)
	 {
		cout << system->networkConnection[i].processID << endl;
	 }

























}
