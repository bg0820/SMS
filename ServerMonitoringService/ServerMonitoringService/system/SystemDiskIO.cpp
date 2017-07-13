#include "SystemDiskIO.hpp"

void SystemDiskIO::CallbackProc()
{
	for (int i = 0; i < this->diskCount; i++) // Disk Count Loop
	{
		if (this->diskList[i].type != DRIVE_REMOVABLE) // Drive connect check
		{
			getDiskPerformance(this->diskList[i].path, this->diskList[i].curDiskPerformance);

			LONGLONG readSec = this->diskList[i].curDiskPerformance.BytesRead.QuadPart - this->diskList[i].prevDiskPerformance.BytesRead.QuadPart;
			LONGLONG writeSec = this->diskList[i].curDiskPerformance.BytesWritten.QuadPart - this->diskList[i].prevDiskPerformance.BytesWritten.QuadPart;

			if (this->diskList[i].prevDiskPerformance.BytesRead.QuadPart >= 0)
			{
				this->diskList[i].readSec = readSec;
				this->diskList[i].writeSec = writeSec;
			}

			// Prev DiskPerformance Data Change
			this->diskList[i].prevDiskPerformance = this->diskList[i].curDiskPerformance;
		}
	}
}

int SystemDiskIO::getPartitionList(vector<string> &paramStrDiskList, Disk *&paramDiskList)
{
	paramStrDiskList.clear();

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
		paramStrDiskList.push_back(szBuffer + (i * 4));

	paramDiskList = new Disk[paramStrDiskList.size()];

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

	return 1;
}

int SystemDiskIO::Start()
{
	if (this->tqTimer == NULL)
	{
		this->tqTimer = new TQTimer(std::bind(&this->CallbackProc, this));
		this->tqTimer->setInterval(1000); // 1Sec
		this->tqTimer->Start();
	}

	return 1;
}

int SystemDiskIO::Stop()
{
	if (this->tqTimer)
	{
		this->tqTimer->Stop();
		delete this->tqTimer;
		this->tqTimer = nullptr;
	}

	return 1;
}

int SystemDiskIO::getListCount()
{
	return this->strDiskList.size();
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
	count = this->strDiskList.size();

	return this->diskList;
}

// call update function after recall getDiskList() function
int SystemDiskIO::Update()
{
	int nResult = 1;

	if (this->diskList)
	{
		delete[] this->diskList;
		this->diskList = nullptr;
	}

	nResult = getPartitionList(this->strDiskList, this->diskList);
	diskCount = strDiskList.size();

	if (nResult)
	{
		for (int i = 0; i < this->strDiskList.size(); i++)
		{
			// nResult = 0; if Then nResult == false 
			if (!getDiskInfo(this->strDiskList[i].c_str(), this->diskList[i]))
				nResult = 0;
		}
	}

	return nResult;
}