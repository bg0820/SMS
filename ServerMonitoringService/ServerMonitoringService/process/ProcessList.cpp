#include "ProcessList.hpp"

void ProcessList::Update()
{
	// Clear();
	hWndList.clear();
	threadCounting.clear();

	// Update
	getProcessList();
	getThreadCount();
	getHwndList();
}

void ProcessList::getProcessList()
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
#include "../util/StopWatch.hpp"

int ProcessList::getThreadCount()
{
	StopWatch sw;
	sw.Start();
	int nResult = 1;
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32 te32;

	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

	if (hThreadSnap == INVALID_HANDLE_VALUE)
		nResult = -1;

	// Fill in the size of the structure before using it. 
	te32.dwSize = sizeof(THREADENTRY32);

	// Retrieve information about the first thread,
	// and exit if unsuccessful
	if (!Thread32First(hThreadSnap, &te32))
	{
		if (hThreadSnap != INVALID_HANDLE_VALUE)
			CloseHandle(hThreadSnap);     // Must clean up the snapshot object!
		nResult = -2;
	}

	// Now walk the thread list of the system,
	// and display information about each thread
	// associated with the specified process
	do // Thread32First
	{
		int pid = te32.th32OwnerProcessID;
		if (isContainsKey(MAP_THREADCOUNT, pid)) // pid exist, 존재 하는 경우
			threadCounting.find(pid)->second++; // +1			
		else // 존재하지 않는 경우
			threadCounting.insert(unordered_map<int, int>::value_type(pid, 1)); // first found = 1

		//printf(TEXT("\n     THREAD ID      = 0x%08X"), te32.th32ThreadID);
		//printf(TEXT("\n     base priority  = %d"), te32.tpBasePri);
		//printf(TEXT("\n     delta priority = %d"), te32.tpDeltaPri);
	} while (Thread32Next(hThreadSnap, &te32));

	//  Don't forget to clean up the snapshot object.
	if (hThreadSnap != INVALID_HANDLE_VALUE)
		CloseHandle(hThreadSnap);

	sw.Stop();
	sw.milliPrint();

	return nResult;
}

void ProcessList::getHwndList()
{
	HWND hWnd = FindWindow(NULL, NULL);
	while (hWnd != NULL)
	{
		if (GetParent(hWnd) == NULL) {
			DWORD dwProcId;
			GetWindowThreadProcessId(hWnd, &dwProcId);

			int pid = dwProcId;
			hWndList.insert(unordered_map<int, HWND>::value_type(pid, hWnd));
		}
		hWnd = GetWindow(hWnd, GW_HWNDNEXT);
	}
}

int ProcessList::getProcessThreadCount(const int paramPID)
{
	int threadCount = 0;

	if (isContainsKey(MAP_THREADCOUNT, paramPID))
		threadCount = threadCounting.find(paramPID)->second;

	return threadCount;
}

HWND ProcessList::getProcessHwnd(const int paramPID)
{
	HWND hWnd = NULL;

	if (isContainsKey(MAP_HWNDLIST, paramPID))
		hWnd = hWndList.find(paramPID)->second;

	return hWnd;
}

int ProcessList::getTotalThreadCount()
{
	return this->threadCounting.size();
}

int ProcessList::getTotalHwndCount()
{
	return this->hWndList.size();
}

BOOLEAN ProcessList::isContainsKey(const int mapType, int paramPID)
{
	if (mapType == MAP_THREADCOUNT)
	{
		if (threadCounting.find(paramPID) == threadCounting.end())
			return FALSE;
		else
			return TRUE;
	}
	else if(mapType == MAP_HWNDLIST)
	{
		if (hWndList.find(paramPID) == hWndList.end())
			return FALSE;
		else
			return TRUE;
	}
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