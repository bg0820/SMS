#ifndef _SYSTEMDISKIO_H_
#define _SYSTEMDISKIO_H_

#include <iostream>
#include <Windows.h>
#include <vector>

#define INTERVAL 500

using namespace std;

typedef unsigned long       DWORD;
typedef struct disk
{
	TCHAR path[MAX_PATH];
	TCHAR name[MAX_PATH];
	TCHAR fileFormat[MAX_PATH];
	ULONGLONG usedBytes = 0;
	ULONGLONG totalBytes = 0;
	ULONGLONG freeBytes = 0;
	int type;
}Disk;

void stringReplace(string& subject, const string& search, const string& replace);

class SystemDiskIO
{
	typedef struct diskPerf
	{
		TCHAR * path;
		DISK_PERFORMANCE prevDiskPerformance;
		DISK_PERFORMANCE curDiskPerformance;
	}DiskPerformance;

private:
	Disk *diskList = NULL;
	vector<string> strDiskList;
	DiskPerformance * diskPerformance = NULL;
	HANDLE timerQueue = NULL;
private:
	static void CALLBACK TimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
	int getPartitionList(vector<string> &parmStrDiskList, Disk *&parmDiskList);
	int getDiskInfo(const TCHAR *volumePath, Disk &disk);
public:
	// first : return getDiskPartitionList
	// second : 
	// third
	SystemDiskIO()
	{
		update();

		diskPerformance = new DiskPerformance;
	}

	~SystemDiskIO()
	{
		cout << "~SystemDiskIO()" << endl;
		if (diskList)
		{
			delete[] diskList;
			diskList = nullptr;
		}

		if (timerQueue)
		{
			CloseHandle(timerQueue);
			timerQueue = NULL;
		}

		if (diskPerformance)
		{
			delete diskPerformance;
			diskPerformance = nullptr;
		}
	}

	static int getDiskPerformance(TCHAR * path, DISK_PERFORMANCE &diskPerformance);
	
	Disk* getDiskList(int &count);
	int update();

	int Start();
	int Stop();
	int getDiskPerformanceUpdatePerSec();
};

#endif