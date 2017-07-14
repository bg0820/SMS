#include "Process.hpp" 

TCHAR* Process::initName()
{
	if (this->handle == NULL)
		return "";

	int nResult = 1;

	TCHAR szProcessName[MAX_PATH];

	//HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, this->pid);

	if (this->handle != NULL)
	{
		HMODULE hMod;
		DWORD cbNeeded;

		if (!EnumProcessModulesEx(this->handle, &hMod, sizeof(hMod), &cbNeeded, LIST_MODULES_ALL))
			nResult = 0;

		if (GetModuleBaseName(this->handle, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR)) == 0)
			nResult = 0;

	}
	else
		nResult = 0;

	/*if (hProcess != INVALID_HANDLE_VALUE)
		CloseHandle(hProcess);*/

	return szProcessName;
}

TCHAR* Process::initPath()
{
	if (this->handle == NULL)
		return "";

	int nResult = 1;

	TCHAR szProcessPath[MAX_PATH];
	//HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, this->pid);

	if (this->handle != NULL)
	{
		HMODULE hMod;
		DWORD cbNeeded;

		if (!EnumProcessModulesEx(this->handle, &hMod, sizeof(hMod), &cbNeeded, LIST_MODULES_ALL))
			nResult = 0;

		if (GetModuleFileNameEx(this->handle, hMod, szProcessPath, sizeof(szProcessPath) / sizeof(TCHAR)) == 0)
			nResult = 0;
	}
	else
		nResult = 0;

	/*if (hProcess != INVALID_HANDLE_VALUE)
		CloseHandle(hProcess);*/

	return szProcessPath;
}



HICON Process::initIcon(BOOLEAN LargeIcon)
{
	SHFILEINFO shFileInfo;

	ZeroMemory(&shFileInfo, sizeof(SHFILEINFO));

	ULONG iconFlag;
	iconFlag = LargeIcon ? SHGFI_LARGEICON : SHGFI_SMALLICON;

	if (!SHGetFileInfo(this->path, 0, &shFileInfo, sizeof(SHFILEINFO), SHGFI_ICON | iconFlag))
		return NULL;

	return shFileInfo.hIcon;
}

TCHAR* Process::initCommandLine()
{
	if (this->handle == NULL)
		return NULL;

	int nResult = 1;
	PVOID pebAddress;
	PVOID rtlUserProcParamsAddress;
	UNICODE_STRING commandLine;
	TCHAR *commandLineContents;

	_NtQueryInformationProcess NtQueryInformationProcess = (_NtQueryInformationProcess)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess");
	PROCESS_BASIC_INFORMATION processBasicInformation;
	NtQueryInformationProcess(this->handle, 0, &processBasicInformation, sizeof(processBasicInformation), NULL);
	pebAddress = processBasicInformation.PebBaseAddress;

	if (!ReadProcessMemory(this->handle, &(((_PEB*)pebAddress)->ProcessParameters), &rtlUserProcParamsAddress, sizeof(PVOID), NULL))
		nResult = -1;

	if (!ReadProcessMemory(this->handle, &(((_RTL_USER_PROCESS_PARAMETERS*)rtlUserProcParamsAddress)->CommandLine), &commandLine, sizeof(commandLine), NULL))
		nResult = -2;

	commandLineContents = new TCHAR[commandLine.Length];

	if (!ReadProcessMemory(this->handle, commandLine.Buffer, commandLineContents, commandLine.Length, NULL))
		nResult = -3;

	int size = commandLine.Length / 2;
	TCHAR *strCommandLine = new TCHAR[size + 1];
	memset(strCommandLine, NULL, size + 1);

	for (int i = 0; i < commandLine.Length; i++)
	{
		if (commandLineContents[i] != NULL)
			strCommandLine[i / 2] = commandLineContents[i];
	}
	strCommandLine[size] = commandLineContents[commandLine.Length - 1];

	if (commandLineContents)
	{
		delete[] commandLineContents;
		commandLineContents = nullptr;
	}


	if (nResult != 1)
		return NULL;
	//	throw nResult;

	return strCommandLine;
}

TCHAR * Process::initOwner()
{
	if (this->handle == NULL)
		return "";

	int nResult = 1;

	TCHAR ownerName[MAX_PATH];
	HANDLE tokenHandle = 0;
	char name[MAX_PATH], dom[MAX_PATH], tubuf[MAX_PATH], *pret = 0;
	DWORD nlen = MAX_PATH, dlen = MAX_PATH;
	TOKEN_USER *tokenUser = (TOKEN_USER*)tubuf;

	int iUse;

	//open the processes token
	if (!OpenProcessToken(this->handle, TOKEN_QUERY, &tokenHandle))
		nResult = 0;

	//get the SID of the token


	if (!GetTokenInformation(tokenHandle, TokenUser, tokenUser, MAX_PATH, &nlen))
		nResult = 0;

	// domain name of the SID
	if (!LookupAccountSidA(0, tokenUser->User.Sid, name, &nlen, dom, &dlen, (PSID_NAME_USE)&iUse))
		nResult = 0;

	// copy info to our static buffer
	strcpy_s(ownerName, dom);
	strcat_s(ownerName, "/");
	strcat_s(ownerName, name);


	if (tokenHandle)
		CloseHandle(tokenHandle);

	if (!nResult)
		return "";

	return ownerName;
}

tm Process::initCreateTime()
{
	FILETIME createTime, exitTime, kernelTime, userTime;
	GetProcessTimes(this->handle, &createTime, &exitTime, &kernelTime, &userTime);

	FILETIME localFileTime;
	FileTimeToLocalFileTime(&createTime, &localFileTime);
	SYSTEMTIME sysTime;
	FileTimeToSystemTime(&localFileTime, &sysTime);
	struct tm tmTime = { 0 };
	tmTime.tm_year = sysTime.wYear - 1900;
	tmTime.tm_mon = sysTime.wMonth - 1;
	tmTime.tm_mday = sysTime.wDay;
	tmTime.tm_hour = sysTime.wHour;
	tmTime.tm_min = sysTime.wMinute;
	tmTime.tm_sec = sysTime.wSecond;
	tmTime.tm_wday = 0;
	tmTime.tm_yday = 0;
	tmTime.tm_isdst = -1;

	return tmTime;
}

HWND Process::getHwndFromPid()
{
	HWND hWnd = FindWindow(NULL, NULL);
	while (hWnd != NULL)
	{
		if (GetParent(hWnd) == NULL) {
			DWORD dwProcId;
			GetWindowThreadProcessId(hWnd, &dwProcId);

			if (this->pid == dwProcId)
				return hWnd;
		}
		hWnd = GetWindow(hWnd, GW_HWNDNEXT);
	}
	return NULL;
}

// When you are finished with the handle, be sure to close it using the CloseHandle function.
HANDLE Process::getHandleFromPid()
{
	HANDLE handle;
	if ((handle = OpenProcess(MAXIMUM_ALLOWED, false, this->pid)) == NULL)
		return NULL;

	return handle;
}

DWORD Process::getPid()
{
	return this->pid;
}

TCHAR * Process::getName()
{
	if (this->name)
		return this->name;
	else
		return "";
}

TCHAR * Process::getPath()
{
	if (this->path)
		return this->path;
	else
		return "";
}

TCHAR * Process::getCommandLine()
{
	if (this->commandLine)
		return this->commandLine;
	else
		return "";
}

TCHAR * Process::getOwner()
{
	if (this->owner)
		return this->owner;
	else
		return "";
}

HICON Process::getIcon()
{
	return this->icon;
}

BOOLEAN Process::isRunning()
{
	DWORD_PTR dw = 0;

	// TimeOut 2sec
	if (SendMessageTimeout(this->hWnd, NULL, NULL, NULL, SMTO_NORMAL, 2000, &dw))
	{
		return TRUE; // process is running
	}
	else
	{
		if (GetLastError() == ERROR_TIMEOUT) {
			return FALSE; // process is not responsed
		}
	}
}

int Process::getHandleCount(DWORD &val)
{
	DWORD pCount;

	if (GetProcessHandleCount(this->handle, &pCount) == 0)
		return 0;

	val = pCount;

	return 1;
}

int Process::getThreadCount(int &paramTotalThreadCount, int &paramCurrentProcessThreadCount)
{
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32 te32;
	int totalThreadCount = 0;
	int processThreadCount = 0;

	// Take a snapshot of all running threads  
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

	if (hThreadSnap == INVALID_HANDLE_VALUE)
		return 0;

	// Fill in the size of the structure before using it. 
	te32.dwSize = sizeof(THREADENTRY32);

	// Retrieve information about the first thread,
	// and exit if unsuccessful
	if (!Thread32First(hThreadSnap, &te32))
	{
		if (hThreadSnap != INVALID_HANDLE_VALUE)
			CloseHandle(hThreadSnap);     // Must clean up the snapshot object!
		return 0;
	}

	// Now walk the thread list of the system,
	// and display information about each thread
	// associated with the specified process
	do // Thread32First
	{
		totalThreadCount++;
		if (te32.th32OwnerProcessID == this->pid)
		{
			processThreadCount++;
			//printf(TEXT("\n     THREAD ID      = 0x%08X"), te32.th32ThreadID);
			//printf(TEXT("\n     base priority  = %d"), te32.tpBasePri);
			//printf(TEXT("\n     delta priority = %d"), te32.tpDeltaPri);
		}
	} while (Thread32Next(hThreadSnap, &te32));

	//  Don't forget to clean up the snapshot object.
	if (hThreadSnap != INVALID_HANDLE_VALUE)
		CloseHandle(hThreadSnap);

	paramTotalThreadCount = totalThreadCount;
	paramCurrentProcessThreadCount = processThreadCount;

	return 1;
}

int Process::getNetworkUsage(double &val)
{
	return 1;
}

int Process::getMemoryUsage(DWORD &val)
{
	int nResult = 1;

	DWORD dWorkingSetPages[1024 * 128]; // 512KB
	DWORD dPageSize = 0x1000;
	DWORD dSharedPages = 0;
	DWORD dPrivatePages = 0;
	DWORD dPageTablePages = 0;

	//HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, this->pid);

	//if (this->handle == NULL)
	//	nResult = 0;

	if (!QueryWorkingSet(this->handle, dWorkingSetPages, sizeof(dWorkingSetPages)))
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

	//if (hProcess != INVALID_HANDLE_VALUE)
	//	CloseHandle(hProcess);

	return nResult;
}

int Process::getCpuUsage(double &val)
{
	int nResult = 1;

	ULONGLONG LastCycleTime = NULL;
	LARGE_INTEGER LastPCounter;
	LastPCounter.QuadPart = 0; // LARGE_INTEGER Init

							   // cpu get core number
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	int numProcessors = sysInfo.dwNumberOfProcessors;

	//HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, this->pid);

	// if (this->handle == NULL)
	//	nResult = 0;

	int count = 0;
	while (true)
	{
		ULONG64	CycleTime;
		LARGE_INTEGER qpcLastInt;

		if (!QueryProcessCycleTime(this->handle, &CycleTime))
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

		Sleep(1); // QueryPerformanceCounter Function Resolution is 1 microsecond

		count++;
	}

	//if (hProcess != INVALID_HANDLE_VALUE)
	//	CloseHandle(hProcess);

	return nResult;
}

int Compare(const void * val1, const void * val2)
{
	if (*(PDWORD)val1 == *(PDWORD)val2)
		return 0;

	return *(PDWORD)val1 > *(PDWORD)val2 ? 1 : -1;
}