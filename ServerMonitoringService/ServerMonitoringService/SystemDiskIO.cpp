#include "SystemDiskIO.hpp"

int main()
{
	SystemDiskIO system;
	Disk *disk = NULL;
	int count;


	int c = 0;
	while (true)
	{
		system.update();
		disk = system.getDiskList(count);

		cout << disk[0].name << endl;
		c++;
		Sleep(3);
	}
	std::system("pause");
}

int SystemDiskIO::getPartitionList(vector<string> &parmStrDiskList, Disk *&parmDiskList)
{
	parmStrDiskList.clear();

	TCHAR szBuffer[MAX_PATH];
	int count = 0;

	// If the function fails, the return value is zero.
	// If the function succeeds, the return value is the length, in characters, of the strings copied to the buffer,
	// not including the terminating null character. Note that an ANSI-ASCII null character uses one byte, but a Unicode (UTF-16) null character uses two bytes.
	if ((count = GetLogicalDriveStrings(sizeof(szBuffer), szBuffer)) == 0)
		return 0;

	// (A:\\B:\\C:\\D:\\)
	// A:\\, B:\\, C:\\, D:\\ 4byte
	for (int i = 0; i < count / 4; i++)
		parmStrDiskList.push_back(szBuffer + (i * 4));

	parmDiskList = new Disk[parmStrDiskList.size()];

	return 1;
}

int SystemDiskIO::getDiskInfo(const TCHAR *path, Disk &disk)
{
	ULONGLONG available = 0, totoalByte = 0, freeByte = 0;
	DWORD volumeSerialNumber;

	strcpy_s(disk.path, path);
	// Get Drive Type
	disk.type = GetDriveType(path);

	ZeroMemory(disk.name, sizeof(disk.name));
	ZeroMemory(disk.fileFormat, sizeof(disk.fileFormat));

	// GetDiskFreeSpaceEx function is fetched when the drive is connected.
	if (disk.type != DRIVE_REMOVABLE)
	{
		// Success non zero
		if (!GetDiskFreeSpaceEx(path,
			(PULARGE_INTEGER)&available,
			(PULARGE_INTEGER)&totoalByte,
			(PULARGE_INTEGER)&freeByte))
			return -1;

		disk.usedBytes = totoalByte - freeByte;
		disk.totalBytes = totoalByte;
		disk.freeBytes = freeByte;

		if (!GetVolumeInformationA(path, disk.name, sizeof(disk.name) + 1, &volumeSerialNumber, NULL, NULL, disk.fileFormat, sizeof(disk.fileFormat) + 1))
			return -2;
	}

	return 1;
}

int SystemDiskIO::Start()
{
	// TODO :: UpdatePerSec
	return 0;
}

int SystemDiskIO::Stop()
{
	// TODO :: UpdatePerSec Stop and DeleteTimerQueue
	//	DeleteTimerQueue(timer);
	return 0;
}

void stringReplace(string& subject, const string& search, const string& replace)
{
	size_t pos = 0;

	while ((pos = subject.find(search, pos)) != string::npos)
	{
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
}

// C, diskPerf;
int SystemDiskIO::getDiskPerformance(TCHAR * path, DISK_PERFORMANCE &diskPerformance)
{
	int nResult = 1;
	DWORD bytes;

	// REPLACE
	TCHAR path_array[MAX_PATH];
	strcpy_s(path_array, path);
	TCHAR *context = NULL;
	TCHAR * tc = strtok_s(path_array, TEXT(":"), &context);
	string strPath;
	strPath = "\\\\.\\C:";
	stringReplace(strPath, "C", tc);

	HANDLE handle = CreateFile(strPath.c_str(),
		FILE_READ_ATTRIBUTES,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (handle == INVALID_HANDLE_VALUE) {
		cout << "Error opening disk" << endl;
		nResult = 0;
	}

	if (!DeviceIoControl(handle,
		IOCTL_DISK_PERFORMANCE,
		NULL,
		0,
		&diskPerformance,
		sizeof(diskPerformance),
		&bytes,
		NULL))
	{
		cout << "Failure in DeviceIoControl" << endl;
		nResult = 0;
	}

	CloseHandle(handle); // Memory Leak : Solution

	return nResult;
}

Disk * SystemDiskIO::getDiskList(int &count)
{
	count = SystemDiskIO::strDiskList.size();
	return SystemDiskIO::diskList;
}

// updateDiskList(), updateDiskInfo()
int SystemDiskIO::update()
{
	int nResult = 1;

	if (SystemDiskIO::diskList)
	{
		delete[] SystemDiskIO::diskList;
		SystemDiskIO::diskList = nullptr;
	}

	nResult = getPartitionList(SystemDiskIO::strDiskList, SystemDiskIO::diskList);

	if (nResult)
	{
		for (int i = 0; i < SystemDiskIO::strDiskList.size(); i++)
		{
			// nResult = 0; if Then nResult == false 
			if (!getDiskInfo(SystemDiskIO::strDiskList[i].c_str(), SystemDiskIO::diskList[i]))
				nResult = 0;
		}
	}

	return nResult;
}

void CALLBACK SystemDiskIO::TimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	DiskPerformance * diskPerformance = (DiskPerformance*)lpParameter;
	getDiskPerformance("C:\\", diskPerformance->curDiskPerformance);

	LONGLONG readSec = diskPerformance->curDiskPerformance.BytesRead.QuadPart - diskPerformance->prevDiskPerformance.BytesRead.QuadPart;
	LONGLONG writeSec = diskPerformance->curDiskPerformance.BytesWritten.QuadPart - diskPerformance->prevDiskPerformance.BytesWritten.QuadPart;

	if (readSec >= 0 && writeSec >= 0)
	{
		cout << readSec / 1024 << "KB/s" << endl;
		cout << writeSec / 1024 << "KB/s" << endl;
		// varReadSec = readSec;
		// varWriteSec = writeSec;
	}

	// Prev DiskPerformance Data Change
	diskPerformance->prevDiskPerformance = diskPerformance->curDiskPerformance;

}


int SystemDiskIO::getDiskPerformanceUpdatePerSec()
{
	int nResult = 1;

	nResult = 1; // getDiskPerformance(prevDiskPerformance);

	HANDLE timerQueue = CreateTimerQueue();
	HANDLE timer;

	// TODO : ~
	// for(int i = 0; i< diskcount; i++){ 
	// diskPerformance->path = "C:\\";
	if (!CreateTimerQueueTimer(&timer, timerQueue, SystemDiskIO::TimerCallback, diskPerformance, 1000, 1000, WT_EXECUTEDEFAULT))
		nResult = 0;
	// }

	if (timerQueue)
	{
		CloseHandle(timerQueue);
		timerQueue = NULL;
	}

	return nResult;
}