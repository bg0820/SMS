#include "SystemDiskIO.hpp"

void SystemDiskIO::CallbackProc()
{
	for (int i = 0; i < SystemDiskIO::diskCount; i++) // Disk Count Loop
	{
		if (SystemDiskIO::diskList[i].type != DRIVE_REMOVABLE) // Drive connect check
		{
			getDiskPerformance(SystemDiskIO::diskList[i].path, SystemDiskIO::diskList[i].curDiskPerformance);

			LONGLONG readSec = SystemDiskIO::diskList[i].curDiskPerformance.BytesRead.QuadPart - SystemDiskIO::diskList[i].prevDiskPerformance.BytesRead.QuadPart;
			LONGLONG writeSec = SystemDiskIO::diskList[i].curDiskPerformance.BytesWritten.QuadPart - SystemDiskIO::diskList[i].prevDiskPerformance.BytesWritten.QuadPart;

			if (SystemDiskIO::diskList[i].prevDiskPerformance.BytesRead.QuadPart >= 0)
			{
				SystemDiskIO::diskList[i].readSec = readSec;
				SystemDiskIO::diskList[i].writeSec = writeSec;
			}

			// Prev DiskPerformance Data Change
			SystemDiskIO::diskList[i].prevDiskPerformance = SystemDiskIO::diskList[i].curDiskPerformance;
		}
	}
}
//
//int main()
//{
//	SystemDiskIO systemdiskIO;
//	systemdiskIO.Start();
//	int count = 0;
//	Disk *disk = NULL;
//	disk = systemdiskIO.getDiskList(count);
//
//	while (true)
//	{
//		for (int i = 0; i < count; i++)
//		{
//			cout << "Read/sec : " << disk[i].readSec / 1024.0 << "kb/s" << endl;
//			cout << "Write/sec : " << disk[i].writeSec / 1024.0 << "kb/s" << endl;
//		}
//		Sleep(1000);
//	}
//
//	system("pause");
//}

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

	// init DriveType == DRIVE_REMOVABLE
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
	/*
	DEVICE_TRIM_DESCRIPTOR dtr;
	memset(&dtr, 0, sizeof(DEVICE_TRIM_DESCRIPTOR));
	memset(&spq, 0, sizeof(STORAGE_PROPERTY_QUERY));
	spq.PropertyId = StorageDeviceTrimProperty;
	spq.QueryType = PropertyStandardQuery;

	BOOL ret = DeviceIoControl(volhand, IOCTL_STORAGE_QUERY_PROPERTY,
		&spq, sizeof(spq),
		&dtr, sizeof(dtr),
		&dwBytesReturned, FALSE);
	*/

	return 1;
}

int SystemDiskIO::Start()
{
	// TODO :: UpdatePerSec

	if (SystemDiskIO::tqTimer == NULL)
	{
		cout << &SystemDiskIO::CallbackProc << endl;
		SystemDiskIO::tqTimer = new TQTimer(std::bind(&SystemDiskIO::CallbackProc, this));
		SystemDiskIO::tqTimer->setInterval(1000); // 1Sec
		SystemDiskIO::tqTimer->Start();
	}

	return 1;
}

int SystemDiskIO::Stop()
{
	// TODO :: UpdatePerSec Stop and DeleteTimerQueue

	// If the function succeeds, the return value is nonzero.
	// If the function fails, the return value is zero.To get extended error information, call GetLastError.

	if (SystemDiskIO::tqTimer)
	{
		SystemDiskIO::tqTimer->Stop();
		delete SystemDiskIO::tqTimer;
		SystemDiskIO::tqTimer = nullptr;
	}

	return 1;
}

int SystemDiskIO::getListCount()
{
	return SystemDiskIO::strDiskList.size();
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

	if (handle != INVALID_HANDLE_VALUE)
		CloseHandle(handle); // Memory Leak : Solution

	return nResult;
}

Disk * SystemDiskIO::getDiskList(int &count)
{
	count = SystemDiskIO::strDiskList.size();
	return SystemDiskIO::diskList;
}

// call update function after recall getDiskList() function
int SystemDiskIO::Update()
{
	int nResult = 1;

	if (SystemDiskIO::diskList)
	{
		delete[] SystemDiskIO::diskList;
		SystemDiskIO::diskList = nullptr;
	}

	nResult = getPartitionList(SystemDiskIO::strDiskList, SystemDiskIO::diskList);
	diskCount = strDiskList.size();

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