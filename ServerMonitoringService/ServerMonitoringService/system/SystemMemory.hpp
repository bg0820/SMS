#ifndef _SYSTEMEMORY_H_
#define _SYSTEMEMORY_H_

#include <Windows.h>

class SystemMemory
{
private:
	MEMORYSTATUSEX memoryStat;
private:
	int memoryCall();
public:
	int getLoadPercent(int &val);
	int getUsedByte(DWORDLONG &val);
	int getTotalByte(DWORDLONG &val);
	int getFreeByte(DWORDLONG &val);
};

#endif