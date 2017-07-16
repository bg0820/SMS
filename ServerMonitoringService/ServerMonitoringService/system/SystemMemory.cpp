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

int SystemMemory::getTotalByte(DWORDLONG &val)
{
	if (!memoryCall())
		return 0;

	val = memoryStat.ullTotalPhys;

	return 1;
}

int SystemMemory::getFreeByte(DWORDLONG &val)
{
	if (!memoryCall())
		return 0;

	val = memoryStat.ullAvailPhys;

	return 1;
}

int SystemMemory::getUsedByte(DWORDLONG &val)
{
	if (!memoryCall())
		return 0;

	// Total - Free = Use
	val = memoryStat.ullTotalPhys - memoryStat.ullAvailPhys;

	return 1;
}
