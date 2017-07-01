#include "SystemDiskIO.hpp"

int SystemDiskIO::getDiskPartitionList(vector<string> & volumeList)
{
	TCHAR szBuffer[MAX_PATH];
	int count = 0;

	if ((count = GetLogicalDriveStrings(sizeof(szBuffer), szBuffer)) == 0)
		return 0;

	for (int i = 0; i < count / 4; i++)
		volumeList.push_back(szBuffer + (i * 4));

	return 1;
}

int SystemDiskIO::getDiskInformation(const TCHAR *volumePath, Disk &disk)
{
	ULONGLONG available, totoalByte, freeByte;

	disk.szVolumePath = volumePath;
	// Get Drive Type
	disk.type = GetDriveType(volumePath);
	// Success non zero
	if (!GetDiskFreeSpaceEx(volumePath,
		(PULARGE_INTEGER)&available,
		(PULARGE_INTEGER)&totoalByte,
		(PULARGE_INTEGER)&freeByte))
		return 0;

	disk.usedBytes = totoalByte - freeByte;
	disk.totalBytes = totoalByte;
	disk.freeBytes = freeByte;

	disk.szVolumeName = new TCHAR[MAX_PATH];
	disk.szFileFormat = new TCHAR[MAX_PATH];

	if (!GetVolumeInformation(volumePath, disk.szVolumeName, sizeof(disk.szVolumeName) + 1, NULL, NULL, NULL, disk.szFileFormat, sizeof(disk.szFileFormat)))
		return 0;

	return 1;
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
	TCHAR * tc = strtok_s(path_array, ":", &context);
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

void CALLBACK SystemDiskIO::TimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	DiskPerformance * diskPerformance = (DiskPerformance*)lpParameter;
	SystemDiskIO::getDiskPerformance("C:", diskPerformance->curDiskPerformance);

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

	// for(int i = 0; i< diskcount; i++){ 
	// diskPerformance->path = "C:\\";
	if (!CreateTimerQueueTimer(&timer, timerQueue, SystemDiskIO::TimerCallback, diskPerformance, 1000, 1000, WT_EXECUTEDEFAULT))
		nResult = 0;
	// }
	return nResult;
}

void SystemDiskIO::destroy(Disk *disk)
{
	delete[] disk->szVolumeName;
	delete[] disk->szFileFormat;
	delete[] disk;

	if (diskPerformance != NULL)
		delete diskPerformance;
}
