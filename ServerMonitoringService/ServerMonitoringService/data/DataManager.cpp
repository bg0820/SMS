#include "DataManager.hpp"

int main()
{
	DataManager *dataManager = new DataManager();
	SystemInfo* systemInfo = dataManager->getSystem();

	system("pause");
}

void DataManager::Update()
{
	StopWatch stopWatch;
	stopWatch.Start();

	if (systemInfo->process)
	{
		for (int i = 0; i < systemInfo->processCount; ++i)
		{
			if (systemInfo->process[i])
				delete systemInfo->process[i];
		}

		delete[] systemInfo->process;
	}

	// ProcessList Update
	processListObj.Update();
	systemInfo->processCount = processListObj.getCount();


	// Process Update
	systemInfo->process = new Process*[systemInfo->processCount];
	for (int i = 0; i < systemInfo->processCount; i++)
	{
		int pid = processListObj.getPID(i);
		systemInfo->process[i] = new Process(pid);
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

	stopWatch.Stop();
	stopWatch.secPrint();
}

void DataManager::CallbackProc()
{
	cout << "UPDATE" << endl;
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
