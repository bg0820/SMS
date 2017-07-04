#include "SystemMemory.hpp"

int SystemMemory::memoryCall()
{
	memoryStat.dwLength = sizeof(MEMORYSTATUSEX);

	if (GlobalMemoryStatusEx(&memoryStat) == 0)
		return  0;

	return 1;
}

int SystemMemory::getLoadPercent(int &val)
{
	if (!memoryCall())
		return  0;

	val = memoryStat.dwMemoryLoad;

	return 1;
}

int SystemMemory::getTotalByte(DWORD &val)
{
	if (!memoryCall())
		return 0;

	val = (double)memoryStat.ullTotalPhys;

	return 1;
}

int SystemMemory::getFreeByte(DWORD &val)
{
	if (!memoryCall())
		return 0;

	val = (double)memoryStat.ullAvailPhys;

	return 1;
}

int SystemMemory::getUsage(double &val)
{
	if (!memoryCall())
		return 0;

	// Total - Free = Use
	val = (double)(memoryStat.ullTotalPhys - memoryStat.ullAvailPhys);

	return 1;
}
