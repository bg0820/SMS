#include "Process.hpp" 

void Process::initFileNamePath()
{
	if (this->handle == NULL)
	{
		this->name = NULL;
		this->path = NULL;
		return;
	}

	int nResult = 1;

	TCHAR szProcessPath[MAX_PATH];
	TCHAR szProcessName[MAX_PATH];

	if (this->handle != NULL)
	{
		HMODULE hMod;
		DWORD cbNeeded;

		if (!EnumProcessModulesEx(this->handle, &hMod, sizeof(hMod), &cbNeeded, LIST_MODULES_ALL))
			nResult = -1;

		// Get Process Path
		if (GetModuleFileNameEx(this->handle, hMod, szProcessPath, sizeof(szProcessPath) / sizeof(TCHAR)) == 0)
			nResult = -2;

		// Get Process Name
		if (GetModuleBaseName(this->handle, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR)) == 0)
			nResult = -3;
	}
	else
		nResult = -4;

	this->name = new TCHAR[MAX_PATH];
	this->path = new TCHAR[MAX_PATH];
	strcpy_s(this->name, MAX_PATH, szProcessName);
	strcpy_s(this->path, MAX_PATH, szProcessPath);
}

void Process::initCommandLine()
{
	if (this->handle == NULL)
		return;

	int nResult = 1;
	PVOID pebAddress;
	PVOID rtlUserProcParamsAddress;
	UNICODE_STRING commandLine;

	_NtQueryInformationProcess NtQueryInformationProcess = (_NtQueryInformationProcess)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess");
	PROCESS_BASIC_INFORMATION processBasicInformation;
	NtQueryInformationProcess(this->handle, 0, &processBasicInformation, sizeof(processBasicInformation), NULL);
	pebAddress = processBasicInformation.PebBaseAddress;

	if (!ReadProcessMemory(this->handle, &(((_PEB*)pebAddress)->ProcessParameters), &rtlUserProcParamsAddress, sizeof(PVOID), NULL))
		nResult = -1;

	if (!ReadProcessMemory(this->handle, &(((_RTL_USER_PROCESS_PARAMETERS*)rtlUserProcParamsAddress)->CommandLine), &commandLine, sizeof(commandLine), NULL))
		nResult = -2;

	TCHAR *commandLineContents = new TCHAR[commandLine.Length];
	if (!ReadProcessMemory(this->handle, commandLine.Buffer, commandLineContents, commandLine.Length, NULL))
		nResult = -3;

	int size = commandLine.Length / 2;
	TCHAR *strCommandLine = new TCHAR[size + 1];
	memset(strCommandLine, NULL, size + 1);

	// char <-> w_char compatibility
	for (int i = 0; i < commandLine.Length; i++)
	{
		if (commandLineContents[i] != NULL)
			strCommandLine[i / 2] = commandLineContents[i];
	}

	if (commandLineContents)
	{
		delete[] commandLineContents;
		commandLineContents = nullptr;
	}

	if (nResult != 1)
		return;
	//	throw nResult;

	this->commandLine = strCommandLine;
}

void Process::initOwner()
{
	if (this->handle == NULL)
	{
		this->owner = NULL;
		return;
	}

	int nResult = 1;

	HANDLE tokenHandle = 0;
	TCHAR name[MAX_PATH], dom[MAX_PATH], tubuf[MAX_PATH];
	DWORD nlen = MAX_PATH, dlen = MAX_PATH;
	TOKEN_USER *tokenUser = (TOKEN_USER*)tubuf;

	//open the processes token
	if (!OpenProcessToken(this->handle, TOKEN_QUERY, &tokenHandle))
		nResult = -1;

	//get the SID of the token
	if (!GetTokenInformation(tokenHandle, TokenUser, tokenUser, MAX_PATH, &nlen))
		nResult = -2;

	// domain name of the SID
	int iUse;
	if (!LookupAccountSidA(0, tokenUser->User.Sid, name, &nlen, dom, &dlen, (PSID_NAME_USE)&iUse))
		nResult = -3;

	this->owner = new TCHAR[MAX_PATH];
	// copy info to our static buffer
	strcpy_s(this->owner, MAX_PATH, dom);
	strcat_s(this->owner, MAX_PATH, "/");
	strcat_s(this->owner, MAX_PATH, name);


	if (tokenHandle)
		CloseHandle(tokenHandle);
}

void Process::initCreateTime()
{
	FILETIME createTime, exitTime, kernelTime, userTime;
	FILETIME localFileTime;
	SYSTEMTIME sysTime;

	GetProcessTimes(this->handle, &createTime, &exitTime, &kernelTime, &userTime);
	FileTimeToLocalFileTime(&createTime, &localFileTime);
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

	this->createTime = tmTime;
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

HICON Process::getIcon(BOOLEAN LargeIcon)
{
	/*
		Get High Resolution ICON
		Vista and higer, 256 * 256
		lower 48 * 48
	*/

	SHFILEINFO sfi;
	if (!SHGetFileInfo(this->path, 0, &sfi, sizeof(sfi), SHGFI_SYSICONINDEX))
		return NULL;

	// Retrieve the system image list.
	// To get the 48x48 icons, use SHIL_EXTRALARGE
	// To get the 256x256 icons (Vista only), use SHIL_JUMBO
	HIMAGELIST* imageList;
	if (SHGetImageList(SHIL_JUMBO, IID_IImageList, (void**)&imageList) != S_OK)
		return NULL;

	// Get the icon we need from the list. Note that the HIMAGELIST we retrieved
	// earlier needs to be casted to the IImageList interface before use.
	HICON hIcon;
	HRESULT hResult = ((IImageList*)imageList)->GetIcon(sfi.iIcon, ILD_TRANSPARENT, &hIcon);

	if (hResult == S_OK)
		return hIcon;

	return NULL;
}

BOOLEAN Process::isRunning()
{
	DWORD_PTR dw = 0;
	HWND hWnd = getHwndFromPid();

	// TimeOut 2sec
	if (SendMessageTimeout(hWnd, NULL, NULL, NULL, SMTO_NORMAL, 2000, &dw))
	{
		return TRUE; // process is running
	}
	else
	{
		if (GetLastError() == ERROR_TIMEOUT) {
			return FALSE; // process is not responsed
		}
	}
	return TRUE;
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
	// TODO : 
	return 1;
}

/*
	val = kbyte
	Not working properly in operate on 64bit system
*/
int Process::getMemoryUsage(DWORD &val)
{
	DWORD workingSetPages[1024 * 128]; // 512KB
	DWORD pageSize = 0x1000;

	DWORD sharedPages = 0;
	DWORD privatePages = 0;
	DWORD pageTablePages = 0;

	if (this->handle == NULL)
		return -1;

	if (!QueryWorkingSet(this->handle, workingSetPages, sizeof(workingSetPages)))
		return -2;

	DWORD pages = workingSetPages[0];
	qsort(&workingSetPages[1], pages, sizeof(DWORD), Compare);

	for (DWORD i = 1; i <= pageSize; i++)
	{
		DWORD currentPageStatus = 0;
		DWORD currentPageAddress;
		DWORD nextPageAddress;
		DWORD nextPageFlags;
		ULONGLONG pageAddress = workingSetPages[i] & 0xFFFFF000;
		ULONGLONG pageFlags = workingSetPages[i] & 0x00000FFF;

		while (i <= pages) // iterate all pages
		{
			currentPageStatus++;

			if (i == pages) //if last page
				break;

			currentPageAddress = workingSetPages[i] & 0xFFFFF000;
			nextPageAddress = workingSetPages[i + 1] & 0xFFFFF000;
			nextPageFlags = workingSetPages[i + 1] & 0x00000FFF;

			//decide whether iterate further or exit
			//(this is non-contiguous page or have different flags) 
			if ((nextPageAddress == (currentPageAddress + pageSize)) && (nextPageFlags == pageFlags))
				i++;
			else
				break;
		}

		if ((pageAddress < 0xC0000000) || (pageAddress > 0xE0000000))
		{
			if (pageFlags & 0x100) // this is shared one
				sharedPages += currentPageStatus;

			else // private one
				privatePages += currentPageStatus;
		}
		else
			pageTablePages += currentPageStatus; //page table region 
	}

	DWORD totalMemory = pages * 4;
	DWORD sharedMemory = sharedPages * 4;
	DWORD privateMemory = totalMemory - sharedMemory;

	cout << "totalMemory : " << totalMemory << endl;
	cout << "sharedMemory : " << sharedMemory << endl;
	cout << "privateMemory : " << privateMemory << endl;

	val = privateMemory; //ref

	return 1;
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

	return nResult;
}

int Compare(const void * val1, const void * val2)
{
	if (*(PDWORD)val1 == *(PDWORD)val2)
		return 0;

	return *(PDWORD)val1 > *(PDWORD)val2 ? 1 : -1;
}