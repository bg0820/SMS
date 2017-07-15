#include "DataManager.hpp"
/*
int main()
{
	// 메모리 누수 디버깅
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// 메모리 누수 찾기
	// _CrtSetBreakAlloc(165);
#endif

	DataManager dataManager = DataManager();
	system("pause");
}
*/

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
			if (systemInfo->process[i])
			{
				delete systemInfo->process[i];
				systemInfo->process[i] = nullptr;
			}
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
		systemInfo->process[i]->getCpuUsage(cpuVal); // 1.7 Sec
	    // systemInfo->process[i]->getMemoryUsage(memoryVal); // 0.6 Sec
		systemInfo->process[i]->getHandleCount(handleCount);
		processListObj.getProcessThreadCount(pid);
		processListObj.getTotalThreadCount();

		/*cout << "Owner : " << systemInfo->process[i]->getOwner() << endl;
		cout << "Name : " << systemInfo->process[i]->getName()  << endl;
		cout << "Path : " << systemInfo->process[i]->getPath() << endl;
		cout << "Cmd : " << systemInfo->process[i]->getCommandLine() << endl;
		cout << "Cpu Usage : " << cpuVal << endl;
		cout << "Memory Usage : " << memoryVal  / 1024.0 << "MB" << endl;
		cout << "Handle Count : " << handleCount << endl;
		cout << "Thread Count : " << processListObj.getProcessThreadCount(pid) << endl;
		cout << "Total Thread Count : " << processListObj.getTotalThreadCount() << endl;
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
		this->tqTimer->setInterval(1500); // 5Sec
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
