#include "Process.h" 

#pragma region GET/SET

DWORD Process::getPID()
{
	return pid;
}

TCHAR* Process::getName()
{
	return name;
}

TCHAR* Process::getPath()
{
	return path;
}

TCHAR* Process::getCommandLine()
{
	return commandLine;
}

int Process::getThreadCount()
{
	return threadCount;
}

int Process::getProcessCount()
{
	return processCount;
}

DWORD Process::getMemoryUsage()
{
	return memoryVal;
}

double Process::getCpuUsage()
{
	return cpuVal;
}

long Process::getIORead()
{
	return ioRead;
}

long Process::getIOWrite()
{
	return ioWrite;
}

long Process::getNetworkSendByte()
{
	return networkSendByte;
}

long Process::getNetworkReceiveByte()
{
	return networkReceiveByte;
}

void Process::setPid(DWORD val)
{
	pid = val;
}

void Process::setName(TCHAR * val)
{
	name = val;
}

void Process::setPath(TCHAR * val)
{
	path = val;
}

void Process::setCommandLine(TCHAR * val)
{
	commandLine = val;
}

void Process::setThreadCount(int val)
{
	threadCount = val;
}

void Process::setProcessCount(int val)
{
	processCount = val;
}

void Process::setMemoryUsage(DWORD val)
{
	memoryVal = val;
}

void Process::setCpuUsage(double val)
{
	cpuVal = val;
}

void Process::setIORead(long val)
{
	ioRead = val;
}

void Process::setIOWrite(long val)
{
	ioWrite = val;
}

void Process::setNetworkSendByte(long val)
{
	networkSendByte = val;
}

void Process::setNetworkReceiveByte(long val)
{
	networkReceiveByte = val;
}

#pragma endregion

const TCHAR* Process::getName(const DWORD processId)
{
	int nResult = 1;

	TCHAR szProcessName[MAX_PATH];
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);

	if (hProcess != NULL)
	{
		HMODULE hMod;
		DWORD cbNeeded;

		if (!EnumProcessModulesEx(hProcess, &hMod, sizeof(hMod), &cbNeeded, LIST_MODULES_ALL))
			nResult = 0;


		if (GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR)) == 0)
			nResult = 0;
	}
	else
	{
		nResult = 0;
	}

	CloseHandle(hProcess);

	return szProcessName;
}

const TCHAR* Process::getPath(const DWORD processId)
{
	int nResult = 1;

	TCHAR szProcessPath[MAX_PATH];
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);

	if (hProcess != NULL)
	{
		HMODULE hMod;
		DWORD cbNeeded;

		if (!EnumProcessModulesEx(hProcess, &hMod, sizeof(hMod), &cbNeeded, LIST_MODULES_ALL))
			nResult = 0;


		if (GetModuleFileNameEx(hProcess, hMod, szProcessPath, sizeof(szProcessPath) / sizeof(TCHAR)) == 0)
			nResult = 0;
	}
	else
	{
		nResult = 0;
	}

	CloseHandle(hProcess);

	return szProcessPath;
}

const TCHAR* Process::getCommandLine(const DWORD processId)
{

}
int Process::getNetworkUsage(const DWORD processId, double &val)
{

}

int Process::getMemoryUsage(const DWORD processId, DWORD &val)
{
	int nResult = 1;

	DWORD dWorkingSetPages[1024 * 128]; // 512KB
	DWORD dPageSize = 0x1000;
	DWORD dSharedPages = 0;
	DWORD dPrivatePages = 0;
	DWORD dPageTablePages = 0;

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);

	if (hProcess == NULL)
		nResult = 0;

	if (!QueryWorkingSet(hProcess, dWorkingSetPages, sizeof(dWorkingSetPages)))
		nResult = 0;

	DWORD dPages = dWorkingSetPages[0];

	qsort(&dWorkingSetPages[1], dPages, sizeof(DWORD), Compare);

	for (DWORD i = 1; i <= dPages; i++)
	{
		DWORD dCurrentPageStatus = 0;
		DWORD dCurrentPageAddress;
		DWORD dNextPageAddress;
		DWORD dNextPageFlags;
		DWORD dPageAddress = dWorkingSetPages[i] & 0xFFFFF000;
		DWORD dPageFlags = dWorkingSetPages[i] & 0x00000FFF;

		while (i <= dPages)   // iterate all pages
		{
			dCurrentPageStatus++;

			if (i == dPages)  //if last page
				break;

			dCurrentPageAddress = dWorkingSetPages[i] & 0xFFFFF000;
			dNextPageAddress = dWorkingSetPages[i + 1] & 0xFFFFF000;
			dNextPageFlags = dWorkingSetPages[i + 1] & 0x00000FFF;

			//decide whether iterate further or exit(this is non-contiguous page or have different flags) 
			if ((dNextPageAddress == (dCurrentPageAddress + dPageSize)) && (dNextPageFlags == dPageFlags))
				i++;
			else
				break;
		}

		if ((dPageAddress < 0xC0000000) || (dPageAddress > 0xE0000000))
		{
			if (dPageFlags & 0x100)            // this is shared one
				dSharedPages += dCurrentPageStatus;
			else                                // private one
				dPrivatePages += dCurrentPageStatus;
		}
		else
			dPageTablePages += dCurrentPageStatus;  //page table region 
	}

	DWORD dTotal = dPages;
	DWORD dShared = dSharedPages;

	val = (dTotal - dShared) * 4; //ref

	CloseHandle(hProcess);

	return nResult;
}
int Process::getCpuUsage(const DWORD processId, double &val)
{
	int nResult = 1;

	ULONGLONG LastCycleTime = NULL;
	LARGE_INTEGER LastPCounter;
	LastPCounter.QuadPart = 0; // LARGE_INTEGER Init

							   // cpu get core number
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	int numProcessors = sysInfo.dwNumberOfProcessors;

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);

	if (hProcess == NULL)
		nResult = 0;

	int count = 0;
	while (true)
	{
		ULONG64	CycleTime;
		LARGE_INTEGER qpcLastInt;

		if (!QueryProcessCycleTime(hProcess, &CycleTime))
			nResult = 0;

		ULONG64 cycle = CycleTime - LastCycleTime;

		if (!QueryPerformanceCounter(&qpcLastInt))
			nResult = 0;

		double Usage = cycle / ((double)(qpcLastInt.QuadPart - LastPCounter.QuadPart));

		// Scaling
		Usage *= 1.0 / numProcessors;
		Usage *= 0.1;

		LastPCounter = qpcLastInt;
		LastCycleTime = CycleTime;

		if (count > 3)
		{
			val = Usage;
			break;
		}

		Sleep(3);
		count++;
	}

	CloseHandle(hProcess);

	return nResult;
}

int Compare(const void * val1, const void * val2)
{
	if (*(PDWORD)val1 == *(PDWORD)val2)
		return 0;

	return *(PDWORD)val1 > *(PDWORD)val2 ? 1 : -1;
}

/*
//int ProcessData::getProcessData()
//{
//	DWORD *pProcesses = NULL;
//	int pProcessesSize;
//
//	// ���μ��� ��� ��������
//	getProcessList(&pProcesses, &pProcessesSize);
//
//	// ���μ��� ���� ��ü ���� ���μ��� ����Ʈ ���� ��ŭ
//	ProcessInfo *processList = new ProcessInfo[pProcessesSize];
//
//	for (int i = 0; i < pProcessesSize; i++)
//	{
//		processList[i].pid = pProcesses[i];
//
//		TCHAR *processInfo = NULL;
//		if (getProcessInfo(pProcesses[i], &processInfo, 1))
//		{
//			processList[i].Name = processInfo;
//		}
//
//		DWORD memoryVal = 0.0;
//		if (getProcessMemoryUsage(pProcesses[i], &memoryVal))
//		{
//			processList[i].memoryVal = memoryVal;
//
//		}
//
//		double cpuVal = 0.0;
//		if (getProcessCpuUsage(pProcesses[i], &cpuVal))
//		{
//			processList[i].cpuVal = cpuVal;
//
//			cout << "=====================" << endl;
//			cout << "PID : " << processList[i].pid << endl;
//			//cout << "ProcessName : " << processList[i].Name << endl;
//			cout << "Memory Usage : " << processList[i].memoryVal / DIV << "MB" << endl;
//			cout << "CPU : " << processList[i].cpuVal << "%" << endl;
//			cout << "=====================" << endl << endl;
//		}
//
//		delete processInfo;
//	}
//
//	delete pProcesses; // �� �� �׿��ִ� DWORD �迭 ����
//	pProcesses = NULL;
//	return 1;
//}
//*/