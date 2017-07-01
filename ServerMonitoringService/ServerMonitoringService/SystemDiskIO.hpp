#ifndef _SYSTEMDISKIO_H_
#define _SYSTEMDISKIO_H_

#include <iostream>
#include <Windows.h>
#include <vector>

using namespace std;

void stringReplace(string& subject, const string& search, const string& replace);


typedef unsigned long       DWORD;
typedef struct disk
{
	string szVolumePath;
	TCHAR * szVolumeName = NULL;
	TCHAR * szFileFormat = NULL;
	ULONGLONG usedBytes;
	ULONGLONG totalBytes;
	ULONGLONG freeBytes;
	int type;
}Disk;

class SystemDiskIO
{
	typedef struct diskPerf
	{
		TCHAR * path;
		DISK_PERFORMANCE prevDiskPerformance;
		DISK_PERFORMANCE curDiskPerformance;
	}DiskPerformance;

private:
	DiskPerformance * diskPerformance = NULL;
private:
	static void CALLBACK TimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
public:
	SystemDiskIO()
	{
		diskPerformance = new DiskPerformance;
	}
	static int getDiskPerformance(TCHAR * path, DISK_PERFORMANCE &diskPerformance);

	int getDiskPartitionList(vector<string> & volumeList);
	int getDiskInformation(const TCHAR *volumePath, Disk &disk);
	int Start();
	int Stop();
	int getDiskPerformanceUpdatePerSec();
	void destroy(Disk *disk);
};

#endif