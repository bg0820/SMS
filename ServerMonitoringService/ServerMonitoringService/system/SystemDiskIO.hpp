#ifndef _SYSTEMDISKIO_H_
#define _SYSTEMDISKIO_H_

#include <iostream>
#include <vector>
#include "../util/TQTimer.hpp"
#include "../util/Util.hpp"
#include <Windows.h>

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
	DISK_PERFORMANCE curDiskPerformance;
	DISK_PERFORMANCE prevDiskPerformance;
	LONGLONG readSec = 0;
	LONGLONG writeSec = 0;
}Disk;


class SystemDiskIO
{
private:
	UINT diskCount = 0;
	Disk *diskList = NULL;
	vector<string> strDiskList;
	TQTimer *tqTimer = NULL;
private:
	int getPartitionList(vector<string> &paramStrDiskList, Disk *&paramDiskList);
	int getDiskInfo(const TCHAR *volumePath, Disk &disk);
	int getDiskPerformance(TCHAR * path, DISK_PERFORMANCE &diskPerformance);
	void CallbackProc();
public:

	// first : return count for getDiskList()
	// second : update()
	// third : start();
	// fourth : stop();
	SystemDiskIO()
	{
		Update();
	}

	~SystemDiskIO()
	{
		if (diskList)
		{
			delete[] diskList;
			diskList = nullptr;
		}
	}

	Disk* getDiskList(int &count);
	int Update();
	int Start();
	int Stop();
	int getListCount();
};

#endif