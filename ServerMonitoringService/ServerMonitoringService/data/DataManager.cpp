#include "DataManager.hpp"

int main()
{
	// 메모리 누수 디버깅
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// 메모리 누수 찾기
	// _CrtSetBreakAlloc(147);
#endif

	DataManager dataManager = DataManager();

	system("pause");
}

Value DataManager::getJsonDiskList(Document::AllocatorType &alloc)
{
	Value jsonDiskList(kArrayType);
	for (int i = 0; i < systemInfo->diskCount; i++)
	{
		// ERROR : 
		Disk disk = systemInfo->disk[i];

		Value jsonDisk(kObjectType);
		string strDiskPath = disk.path;
		Value vDiskPath(strDiskPath.c_str(), strDiskPath.size(), alloc);
		string strDiskName = disk.name;
		Value vDiskName(strDiskPath.c_str(), strDiskPath.size(), alloc);
		string strDiskFileFormat = disk.fileFormat;
		Value vDiskFileFormat(strDiskPath.c_str(), strDiskPath.size(), alloc);
		jsonDisk.AddMember("Path", vDiskPath, alloc);
		jsonDisk.AddMember("Name", vDiskName, alloc);
		jsonDisk.AddMember("FileFormat", vDiskFileFormat, alloc);
		jsonDisk.AddMember("UsedBytes", disk.usedBytes, alloc);
		jsonDisk.AddMember("TotalBytes", disk.totalBytes, alloc);
		jsonDisk.AddMember("FreeBytes", disk.freeBytes, alloc);
		jsonDisk.AddMember("Type", disk.type, alloc);

		DISK_PERFORMANCE diskPerformance = disk.curDiskPerformance;
		Value jsonDiskPerformance(kObjectType);
		jsonDiskPerformance.AddMember("BytesRead", diskPerformance.BytesRead.QuadPart, alloc);
		jsonDiskPerformance.AddMember("BytesWritten", diskPerformance.BytesWritten.QuadPart, alloc);
		jsonDiskPerformance.AddMember("ReadTime", diskPerformance.ReadTime.QuadPart, alloc);
		jsonDiskPerformance.AddMember("WriteTime", diskPerformance.WriteTime.QuadPart, alloc);
		jsonDiskPerformance.AddMember("IdleTime", diskPerformance.IdleTime.QuadPart, alloc);
		// DWORD > ULONGLONG
		jsonDiskPerformance.AddMember("ReadCount", (ULONGLONG)diskPerformance.ReadCount, alloc);
		jsonDiskPerformance.AddMember("WriteCount", (ULONGLONG)diskPerformance.WriteCount, alloc);
		jsonDiskPerformance.AddMember("QueueDepth", (ULONGLONG)diskPerformance.QueueDepth, alloc);
		jsonDiskPerformance.AddMember("SplitCount", (ULONGLONG)diskPerformance.SplitCount, alloc);
		jsonDiskPerformance.AddMember("QueryTime", diskPerformance.QueryTime.QuadPart, alloc);
		jsonDiskPerformance.AddMember("StorageDeviceNumber", (ULONGLONG)diskPerformance.StorageDeviceNumber, alloc);
		/* Remove
			wstring ws(diskPerformance.StorageManagerName);
			cout << ws.c_str() << endl;
			Value vDiskPerformanceManagerName(wsConvert.c_str(), wsConvert.size(), alloc);
			jsonDiskPerformance.AddMember("StorageManagerName", "D", alloc);
		*/

		jsonDisk.AddMember("DiskPerformance", jsonDiskPerformance, alloc);
		jsonDisk.AddMember("ReadSec", disk.readSec, alloc);
		jsonDisk.AddMember("WriteSec", disk.writeSec, alloc);
		jsonDiskList.PushBack(jsonDisk, alloc);
	}

	return jsonDiskList;
}

Value DataManager::getJsonProcessList(Document::AllocatorType &alloc)
{
	Value jsonProcessList(kArrayType);

	for (int i = 0; i < systemInfo->processCount; i++)
	{
		Process *process = systemInfo->process[i];
		Value jsonProcess(kObjectType);
		jsonProcess.AddMember("PID", (ULONGLONG)process->getPid(), alloc);

		string strCommmand = process->getCommandLine();	
		string strName = process->getName();
		string strPath = process->getPath();	
		string strOwner = process->getOwner();
		Value vCommand(strCommmand.c_str(), strCommmand.size(), alloc);
		Value vName(strName.c_str(), strName.size(), alloc);
		Value vPath(strPath.c_str(), strPath.size(), alloc);
		Value vOwner(strOwner.c_str(), strOwner.size(), alloc);

		jsonProcess.AddMember("CommandLine", vCommand, alloc);
		jsonProcess.AddMember("Name", vName, alloc);
		jsonProcess.AddMember("Path", vPath, alloc);
		jsonProcess.AddMember("Owner", vOwner, alloc);
		DWORD handleCount;
		double cpuUsage;
		process->getHandleCount(handleCount);
		process->getCpuUsage(cpuUsage);
		int threadCount = processListObj.getProcessThreadCount(process->getPid());
		HWND hWnd = processListObj.getProcessHwnd(process->getPid());
		string strCreateTime = process->getCreateTime();
		Value vCreateTime(strCreateTime.c_str(), strCreateTime.size(), alloc);
		jsonProcess.AddMember("CreateTime", vCreateTime, alloc);
		jsonProcess.AddMember("HandleCount", (ULONGLONG)handleCount, alloc);
		jsonProcess.AddMember("ThreadCount", threadCount, alloc);
		//jsonProcess.AddMember("MemoryUsage", 123, alloc);
		jsonProcess.AddMember("CpuUsage", cpuUsage, alloc);
		jsonProcess.AddMember("Running", process->isRunning(hWnd), alloc);
		jsonProcessList.PushBack(jsonProcess, alloc);
	}

	return jsonProcessList;
}

Value DataManager::getJsonNetworkConnectionList(Document::AllocatorType &alloc)
{
	Value jsonNetworkConnectionList(kArrayType);

	for (int i = 0; i < systemInfo->networkConnectionCount; i++)
	{
		NetworkConnection networkConnection = systemInfo->networkConnection[i];
		Value jsonNetworkConnection(kObjectType);
		jsonNetworkConnection.AddMember("ProtocolType", (ULONGLONG)networkConnection.protocolType, alloc);

		Value jsonLocalEndpoint(kObjectType);		
		string strLocalIPv4 = inet_ntoa(networkConnection.localEndpoint.Address.InAddr);
		char localBuf[40]; // temp buffer 39 + 1('\0')
		inet_ntop(AF_INET6, &networkConnection.localEndpoint.Address.In6Addr, localBuf, sizeof(localBuf));
		string strLocalIPv6 = localBuf;
		Value vLocalIPv4(strLocalIPv4.c_str(), strLocalIPv4.size(), alloc);
		Value vLocalIPv6(strLocalIPv6.c_str(), strLocalIPv6.size(), alloc);
		jsonLocalEndpoint.AddMember("IPV4", vLocalIPv4, alloc);
		jsonLocalEndpoint.AddMember("IPV6", vLocalIPv6, alloc);
		jsonLocalEndpoint.AddMember("Port", (ULONGLONG)networkConnection.localEndpoint.Port, alloc);
		jsonNetworkConnection.AddMember("LocalEndpoint", jsonLocalEndpoint, alloc);

		Value jsonRemoteEndpoint(kObjectType);
		string strRemoteIPv4 = inet_ntoa(networkConnection.localEndpoint.Address.InAddr);
		char remoteBuf[40]; // temp buffer 39 + 1('\0')
		inet_ntop(AF_INET6, &networkConnection.localEndpoint.Address.In6Addr, remoteBuf, sizeof(remoteBuf));
		string strRemoteIPv6 = remoteBuf;
		Value vRemoteIPv4(strRemoteIPv4.c_str(), strRemoteIPv4.size(), alloc);
		Value vRemoteIPv6(strRemoteIPv6.c_str(), strRemoteIPv6.size(), alloc);
		jsonRemoteEndpoint.AddMember("IPV4", vRemoteIPv4, alloc);
		jsonRemoteEndpoint.AddMember("IPV6", vRemoteIPv6, alloc);
		jsonRemoteEndpoint.AddMember("Port", (ULONGLONG)networkConnection.remoteEndpoint.Port, alloc);
		jsonNetworkConnection.AddMember("RemoteEndpoint", jsonRemoteEndpoint, alloc);
		

		jsonNetworkConnection.AddMember("State", (ULONGLONG)networkConnection.state, alloc);
		jsonNetworkConnection.AddMember("ProcessID", (ULONGLONG)networkConnection.processID, alloc);
		//jsonNetworkConnection.AddMember("CreateTime", networkConnection.createTime.QuadPart, alloc);
		//jsonNetworkConnection.AddMember("OwnerInfo", networkConnection.ownerInfo, alloc); // TODO : getProcessOwner(networkConnection.processID);
		jsonNetworkConnectionList.PushBack(jsonNetworkConnection, alloc);
	}
	
	return jsonNetworkConnectionList;
}

Value DataManager::getJsonAdapterInfoList(Document::AllocatorType &alloc)
{
	Value jsonAdapterInfoList(kArrayType);

	for (int i = 0; i < systemInfo->adapterCount; i++)
	{
		IP_ADAPTER_INFO adapterInfo = systemInfo->adapterInfo[i];
		Value jsonAdapterInfo(kObjectType);
		jsonAdapterInfo.AddMember("ComboIndex", (ULONGLONG)adapterInfo.ComboIndex, alloc);

		
		string strAdapaterName = adapterInfo.AdapterName;
		string strDescription = adapterInfo.Description;
		Value vAdapterName(strAdapaterName.c_str(), strAdapaterName.size(), alloc);
		Value vDescription(strDescription.c_str(), strDescription.size(), alloc);

		jsonAdapterInfo.AddMember("AdapterName", vAdapterName, alloc);
		jsonAdapterInfo.AddMember("Description", vDescription, alloc);
		jsonAdapterInfo.AddMember("AddressLength", adapterInfo.AddressLength, alloc);
		jsonAdapterInfo.AddMember("Address", 2, alloc);
		jsonAdapterInfo.AddMember("Index", (ULONGLONG)adapterInfo.Index, alloc);
		jsonAdapterInfo.AddMember("Type", adapterInfo.Type, alloc);
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
	}

	return jsonAdapterInfoList;
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
	string strUserName = systemInfo->userName;
	string strComputerName = systemInfo->computerName;
	string strOSVerisonName = systemInfo->osVersion;
	Value vUserName(strUserName.c_str(), strUserName.size(), alloc);
	Value vComputerName(strComputerName.c_str(), strComputerName.size(), alloc);
	Value vOSVersionName(strOSVerisonName.c_str(), strOSVerisonName.size(), alloc);
	json.AddMember("UserName", vUserName, alloc);
	json.AddMember("ComputerName", vComputerName, alloc);
	json.AddMember("OSVersionName", vOSVersionName, alloc);

	// json Disk List
	json.AddMember("Disk List", getJsonDiskList(alloc), alloc);
	json.AddMember("DiskCount", systemInfo->diskCount, alloc);

	// json Process List
	json.AddMember("Process List", getJsonProcessList(alloc), alloc);
	json.AddMember("TotalThreadCount", processListObj.getTotalThreadCount(), alloc);

	// json Network Connection List
	json.AddMember("Network Connection List", getJsonNetworkConnectionList(alloc), alloc);
	json.AddMember("NetworkConnectionCount", (ULONGLONG)systemInfo->networkConnectionCount, alloc);

	// json AdapaterInfo List
	json.AddMember("AdapterInfo List", getJsonAdapterInfoList(alloc), alloc);
	json.AddMember("AdapterCount", systemInfo->adapterCount, alloc);
	
	
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	json.Accept(writer);
	cout << endl << "RAW DATA SIZE : " << buffer.GetSize() << endl;

	int srcSize;
	int compSize;
	TCHAR *compressed = NULL;
	TCHAR *decompressed = NULL;
	cout << "RETURN VALUE : " << Util::Compress(buffer.GetString(), srcSize, compressed, compSize) << endl;
	cout << "COM DATA SIZE : " << compSize << endl;
	cout << "RETURN VALUE : " << Util::DeCompress(compressed, compSize, srcSize, decompressed) << endl;
	cout << "DECOM DATA SIZE : " << strlen(decompressed) << endl;

	// TODO : compressed delete
	// TODO : decompressed delete

	//std::cout << buffer.GetSize() << std::endl;
}

void DataManager::Update()
{

	// ProcessList Update
	processListObj.Update();

	// Memory Leak prevation
	if (systemInfo->process)
	{
		for (int i = 0; i < systemInfo->processCount; i++)
		{
			if (systemInfo->process[i] != NULL)
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

		/*double cpuVal;
		DWORD memoryVal;
		DWORD handleCount;
		systemInfo->process[i]->getCpuUsage(cpuVal); // 1.7 Sec
		// systemInfo->process[i]->getMemoryUsage(memoryVal); // 0.6 Sec
		systemInfo->process[i]->getHandleCount(handleCount);
		processListObj.getProcessThreadCount(pid);
		processListObj.getTotalThreadCount();*/

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
	systemInfo->cpuIdleVal = (100.0 - systemInfo->cpuUsageVal);

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
