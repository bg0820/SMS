#include "DataManager.hpp"

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

void DataManager::jsonUpdate()
{
	// json object create
	Document json;
	Document::AllocatorType &alloc = json.GetAllocator();
	json.SetObject();

	// json Cpu
	json.AddMember("CpuUsage", systemInfo->cpuUsageVal, alloc);
	json.AddMember("CpuIdle", systemInfo->cpuIdleVal, alloc);
	string strCpuModel = systemInfo->cpuModel;
	Value vCpuModel(strCpuModel.c_str(), strCpuModel.size(), alloc);
	json.AddMember("CpuModel", vCpuModel, alloc);

	// json Memory
	json.AddMember("MemoryLoadPercent", systemInfo->memoryLoadPercent, alloc);
	json.AddMember("MemoryUsedByte", systemInfo->memoryUsedByte, alloc);
	json.AddMember("MemoryFreeByte", systemInfo->memoryFreeByte, alloc);
	json.AddMember("MemoryTotalByte", systemInfo->memoryTotalByte, alloc);

	// json System Info
/*	string strUserName = systemInfo->userName;
	string strComputerName = systemInfo->computerName;
	string strOSVerisonName = systemInfo->osVersion;

	json.AddMember("UserName", strUserName, alloc);
	json.AddMember("ComputerName", strComputerName, alloc);
	json.AddMember("OSVersionName", strOSVerisonName, alloc);*/

	// json Disk list
	Value jsonDiskList(kArrayType);
	Value jsonDisk(kObjectType);
	jsonDisk.AddMember("Path", "C:\\", alloc);
	jsonDisk.AddMember("Name", "C:\\", alloc);
	jsonDisk.AddMember("FileFormat", "C:\\", alloc);
	jsonDisk.AddMember("UsedBytes", "C:\\", alloc);
	jsonDisk.AddMember("TotalBytes", "C:\\", alloc);
	jsonDisk.AddMember("FreeBytes", "C:\\", alloc);
	jsonDisk.AddMember("Type", "C:\\", alloc);

	Value jsonDiskPerformance(kObjectType);
	jsonDiskPerformance.AddMember("BytesRead", 123, alloc);
	jsonDiskPerformance.AddMember("BytesWritten", 31, alloc);
	jsonDiskPerformance.AddMember("ReadTime", 123, alloc);
	jsonDiskPerformance.AddMember("WriteTime", 123, alloc);
	jsonDiskPerformance.AddMember("IdleTime", 1234, alloc);
	jsonDiskPerformance.AddMember("ReadCount", 1234, alloc);
	jsonDiskPerformance.AddMember("WriteCount", 1234, alloc);
	jsonDiskPerformance.AddMember("QueueDepth", 1234, alloc);
	jsonDiskPerformance.AddMember("SplitCount", 1234, alloc);
	jsonDiskPerformance.AddMember("QueryTime", 1234, alloc);
	jsonDiskPerformance.AddMember("StorageDeviceNumber", 2, alloc);
	jsonDiskPerformance.AddMember("StorageManagerName", "Name", alloc);
	jsonDisk.AddMember("DiskPerformance", jsonDiskPerformance, alloc);

	jsonDisk.AddMember("ReadSec", 123, alloc);
	jsonDisk.AddMember("WriteSec", 1234, alloc);
	jsonDisk.AddMember("DiskCount", 1234, alloc);
	jsonDiskList.PushBack(jsonDisk, alloc);
	json.AddMember("Disk List", jsonDiskList, alloc);

	Value jsonProcessList(kArrayType);
	Value jsonProcess(kObjectType);
	jsonProcess.AddMember("PID", 123, alloc);
	jsonProcess.AddMember("CommandLine", 123, alloc);
	jsonProcess.AddMember("Name", 123, alloc);
	jsonProcess.AddMember("Path", 123, alloc);
	jsonProcess.AddMember("Owner", 123, alloc);
	jsonProcess.AddMember("CreateTime", 123, alloc);
	jsonProcess.AddMember("HandleCount", 123, alloc);
	jsonProcess.AddMember("ThreadCount", 123, alloc);
	jsonProcess.AddMember("MemoryUsage", 123, alloc);
	jsonProcess.AddMember("CpuUsage", 123, alloc);
	jsonProcessList.PushBack(jsonProcess, alloc);
	json.AddMember("Process List", jsonProcessList, alloc);
	json.AddMember("TotalThreadCount", 2145, alloc);

	Value jsonNetworkConnectionList(kArrayType);
	Value jsonNetworkConnection(kObjectType);
	jsonNetworkConnection.AddMember("ProtocolType", 2, alloc);

	Value jsonLocalEndpoint(kObjectType);
	jsonLocalEndpoint.AddMember("IPV4", 123, alloc);
	jsonLocalEndpoint.AddMember("IPV6", 123, alloc);
	jsonLocalEndpoint.AddMember("Port", 123, alloc);
	jsonNetworkConnection.AddMember("LocalEndpoint", jsonLocalEndpoint, alloc);

	Value jsonRemoteEndpoint(kObjectType);
	jsonRemoteEndpoint.AddMember("IPV4", 123, alloc);
	jsonRemoteEndpoint.AddMember("IPV6", 123, alloc);
	jsonRemoteEndpoint.AddMember("Port", 123, alloc);
	jsonNetworkConnection.AddMember("RemoteEndpoint", jsonRemoteEndpoint, alloc);

	jsonNetworkConnection.AddMember("State", 2, alloc);
	jsonNetworkConnection.AddMember("ProcessID", 2, alloc);
	jsonNetworkConnection.AddMember("CreateTime", 2, alloc);
	jsonNetworkConnection.AddMember("OwnerInfo", 2, alloc);
	jsonNetworkConnectionList.PushBack(jsonNetworkConnection, alloc);
	json.AddMember("NetworkConnection List", jsonNetworkConnectionList, alloc);
	json.AddMember("NetworkConnectionCount", 2, alloc);


	Value jsonAdapterInfoList(kArrayType);
	Value jsonAdapterInfo(kObjectType);
	jsonAdapterInfo.AddMember("ComboIndex", 2, alloc);
	jsonAdapterInfo.AddMember("AdapterName", 2, alloc);
	jsonAdapterInfo.AddMember("Description", 2, alloc);
	jsonAdapterInfo.AddMember("AddressLength", 2, alloc);
	jsonAdapterInfo.AddMember("Address", 2, alloc);
	jsonAdapterInfo.AddMember("Index", 2, alloc);
	jsonAdapterInfo.AddMember("Type", 2, alloc);
	jsonAdapterInfo.AddMember("DhcpEnabled", 2, alloc);
	jsonAdapterInfo.AddMember("CurrentIpAddress", 2, alloc);
	// IP List
	Value jsonIpAddressList(kArrayType);
	Value jsonIpAddress(kObjectType);
	jsonIpAddress.AddMember("IP", "123.123.123.123", alloc);
	jsonIpAddressList.PushBack(jsonIpAddress, alloc);
	jsonAdapterInfo.AddMember("IpAddressList", jsonIpAddressList, alloc);

	// Gateway List
	Value jsonGatewayList(kArrayType);
	Value jsonGateway(kObjectType);
	jsonGateway.AddMember("IP", "123.123.123.123", alloc);
	jsonGatewayList.PushBack(jsonGateway, alloc);
	jsonAdapterInfo.AddMember("GatewayList", jsonGatewayList, alloc);

	jsonAdapterInfo.AddMember("DhcpServer", "LIST", alloc);
	jsonAdapterInfo.AddMember("HaveWins", FALSE, alloc);
	jsonAdapterInfo.AddMember("PrimaryWinsServer", "123.123.123.123", alloc);
	jsonAdapterInfo.AddMember("SecondaryWinsServer", "123.123.123.123", alloc);
	jsonAdapterInfo.AddMember("LeaseObtained", "2017-07-12 12:23:43", alloc);
	jsonAdapterInfo.AddMember("LeaseExpires", "2017-07-12 12:23:43", alloc);

	jsonAdapterInfoList.PushBack(jsonAdapterInfo, alloc);
	json.AddMember("AdapterInfo List", jsonAdapterInfoList, alloc);

	json.AddMember("AdapterCount", 2, alloc);

/*	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	json.Accept(writer);

	std::cout << buffer.GetString() << std::endl;*/
}

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

	// Json Update
	jsonUpdate();

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
