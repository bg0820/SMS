#include "DataManager.hpp"

void DataManager::Update()
{
	// ProcessList Update
	processListObj.Update();
	int processCount = processListObj.getCount;
	processes = new DWORD[processCount];
	// Process Update
	system.process = new Process[processCount];
	for (int i = 0; i < processCount; i++)
	{
		int pid = processListObj.getPID(i);
		system.process[i] = processListObj.getProcess(pid);
	}

}
