#include "ProcessList.hpp"

void ProcessList::Update()
{
	while (true)
	{
		DWORD cb = sizeof(DWORD) * processListCount;
		DWORD cbNeeded = NULL;

		EnumProcesses(this->processList, cb, &cbNeeded);

		if (cb == cbNeeded) // ProcessList array space lack
		{
			if (this->processList)
			{
				delete[] this->processList;
				this->processList = nullptr;
			}

			this->processListCount += 128;
			this->processList = new DWORD[this->processListCount];
		}
		else
		{
			this->processListSize = cbNeeded;
			break;
		}
	}
}

int ProcessList::getThreadCount()
{
	int nResult = 1;
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32 te32;

	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

	if (hThreadSnap == INVALID_HANDLE_VALUE)
		nResult = 0;

	// Fill in the size of the structure before using it. 
	te32.dwSize = sizeof(THREADENTRY32);

	// Retrieve information about the first thread,
	// and exit if unsuccessful
	if (!Thread32First(hThreadSnap, &te32))
	{
		if (hThreadSnap != INVALID_HANDLE_VALUE)
			CloseHandle(hThreadSnap);     // Must clean up the snapshot object!
		nResult = 0;
	}

	// Now walk the thread list of the system,
	// and display information about each thread
	// associated with the specified process
	do // Thread32First
	{
		int pid = te32.th32OwnerProcessID;
		if (isContainsKey(pid)) // pid exist, 존재 하는 경우
		{
			threadCounting.find(pid)->second++; // +1			
		}
		else // 존재하지 않는 경우
		{
			threadCounting.insert(unordered_map<int, int>::value_type(pid, 1)); // first found = 1
		}

		//printf(TEXT("\n     THREAD ID      = 0x%08X"), te32.th32ThreadID);
		//printf(TEXT("\n     base priority  = %d"), te32.tpBasePri);
		//printf(TEXT("\n     delta priority = %d"), te32.tpDeltaPri);
	} while (Thread32Next(hThreadSnap, &te32));

	//  Don't forget to clean up the snapshot object.
	if (hThreadSnap != INVALID_HANDLE_VALUE)
		CloseHandle(hThreadSnap);

	return nResult;
}

BOOLEAN ProcessList::isContainsKey(int paramPID)
{
	if (threadCounting.find(paramPID) == threadCounting.end())
		return FALSE;
	else
		return TRUE;
}

int ProcessList::getProcessThreadCount(const int paramPID)
{
	int threadCount = 0;

	if (isContainsKey(paramPID))
		threadCount = threadCounting.find(paramPID)->second;

	return threadCount;
}

int ProcessList::getCount()
{
	if (processListSize != NULL)
		return  processListSize / sizeof(DWORD);
	else
		return 0;
}

int ProcessList::getPID(const int i)
{
	if (processList[i] != NULL && processList[i] != 0)
		return processList[i];
	else
		return 0;
}