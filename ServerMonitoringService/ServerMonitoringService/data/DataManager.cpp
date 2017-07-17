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

int DataManager::Compress(const TCHAR *src)
{
	StopWatch stopWatch;
	stopWatch.Start();
	// 마지막 NULL 문자
	// Last null char
	const int src_size = (int)(strlen(src) + 1);
	// 압축 된 출력의 최대 크기를 반환
	const int max_dst_size = LZ4_compressBound(src_size);
	// We will use that size for our destination boundary when allocating space.
	TCHAR* compressed_data = new TCHAR[max_dst_size];
	if (compressed_data == NULL)
		return -1; // Failed to allocate memory for *compressed_data.

	const int compressed_data_size = LZ4_compress_default(src, compressed_data, src_size, max_dst_size);

	// Check return_value to determine what happened.
	if (compressed_data_size < 0)
		return -2; // A negative result from LZ4_compress_default indicates a failure trying to compress the data.  See exit code (echo $?) for value returned.
	else if (compressed_data_size == 0)
		return -3; // A result of 0 means compression worked, but was stopped because the destination buffer couldn't hold all the information.
	if (compressed_data_size > 0)
		printf("We successfully compressed some data!\n");

	cout << "COMPRESS DATA SIZE : " << compressed_data_size << endl;

	stopWatch.Stop();
	stopWatch.milliPrint();

	return 1;
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
		// DWORD > UINT
		jsonDiskPerformance.AddMember("ReadCount", (UINT)diskPerformance.ReadCount, alloc);
		jsonDiskPerformance.AddMember("WriteCount", (UINT)diskPerformance.WriteCount, alloc);
		jsonDiskPerformance.AddMember("QueueDepth", (UINT)diskPerformance.QueueDepth, alloc);
		jsonDiskPerformance.AddMember("SplitCount", (UINT)diskPerformance.SplitCount, alloc);
		jsonDiskPerformance.AddMember("QueryTime", diskPerformance.QueryTime.QuadPart, alloc);
		jsonDiskPerformance.AddMember("StorageDeviceNumber", (UINT)diskPerformance.StorageDeviceNumber, alloc);
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
		jsonProcess.AddMember("PID", (UINT)process->getPid(), alloc);

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
		jsonProcess.AddMember("HandleCount", (UINT)handleCount, alloc);
		jsonProcess.AddMember("ThreadCount", threadCount, alloc);
		//jsonProcess.AddMember("MemoryUsage", 123, alloc);
		jsonProcess.AddMember("CpuUsage", cpuUsage, alloc);
		jsonProcess.AddMember("Running", process->isRunning(hWnd), alloc);
		jsonProcessList.PushBack(jsonProcess, alloc);
	}

	return jsonProcessList;
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
	
	
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	json.Accept(writer);

	cout << endl << "RAW DATA SIZE : " << buffer.GetSize() << endl;
	cout << "RETURN VALUE : " <<Compress(buffer.GetString()) << endl;
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
		this->tqTimer->setInterval(100000); // 100Sec
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
