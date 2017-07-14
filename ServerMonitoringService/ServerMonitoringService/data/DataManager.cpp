#include "DataManager.hpp"

void DataManager::Update()
{
	StopWatch stopWatch;
	stopWatch.Start();

	// ProcessList Update
	processListObj.Update();

	// Memory Leak prevation
	if (systemInfo->process)
	{
		for (int i = 0; i < systemInfo->processCount; i++)
		{
			delete systemInfo->process[i];
			systemInfo->process[i] = nullptr;
		}
		
		delete[] systemInfo->process;
		systemInfo->process = nullptr;
	}

	systemInfo->processCount = processListObj.getCount();

	// Process Update
	systemInfo->process = new Process*[systemInfo->processCount];
	for (int i = 0; i < systemInfo->processCount; i++)
	{
		int pid = processListObj.getPID(i);
		systemInfo->process[i] = new Process(pid);
		systemInfo->process[i]->getOwner();
		systemInfo->process[i]->getName();
		systemInfo->process[i]->getPath();
		systemInfo->process[i]->getCommandLine();

		double cpuVal;
		DWORD memoryVal;
		DWORD handleCount;
		int toTalCount, currentCount;
		systemInfo->process[i]->getCpuUsage(cpuVal);
		systemInfo->process[i]->getMemoryUsage(memoryVal);
		systemInfo->process[i]->getHandleCount(handleCount);
		systemInfo->process[i]->getThreadCount(toTalCount, currentCount);

		/*cout << "Owner : " << systemInfo->process[i]->getOwner() << endl;
		cout << "Name : " << systemInfo->process[i]->getName()  << endl;
		cout << "Path : " << systemInfo->process[i]->getPath() << endl;
		cout << "Cmd : " << systemInfo->process[i]->getCommandLine() << endl;
		cout << "=========================================" << endl<< endl;*/
	}

	// Cpu Usage Update
	systemCpu.getUsage(systemInfo->cpuUsageVal);
	systemCpu.getIdleUsage(systemInfo->cpuIdleVal);

	// Disk Usage Update
	systemInfo->disk = systemDiskIO.getDiskList(systemInfo->diskCount);
	systemDiskIO.Update();

	// Memory Update
	systemMemory.getFreeByte(systemInfo->memoryFreeByte);
	systemMemory.getUsedByte(systemInfo->memoryUsedByte);
	systemMemory.getTotalByte(systemInfo->memoryTotalByte);
	systemMemory.getLoadPercent(systemInfo->memoryLoadPercent);

	// Network Update

	// Memory Leak prevation
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

	systemNetwork.getConnectionTable(systemInfo->networkConnection, systemInfo->networkConnectionCount);
	systemNetwork.getInterfaces(systemInfo->adapterInfo, systemInfo->adapterCount);

	stopWatch.Stop();
	stopWatch.secPrint();
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
