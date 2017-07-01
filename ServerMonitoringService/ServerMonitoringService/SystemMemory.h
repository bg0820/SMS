#ifndef _SYSTEMEMORY_H_
#define _SYSTEMEMORY_H_

#include "windows.h"

typedef unsigned long       DWORD;

class SystemMemory
{
private:
	MEMORYSTATUSEX memoryStat;
private:
	int memoryCall();
public:
	int getLoadPercent(int &val);
	int getUsage(double &val);
	int getTotalByte(DWORD &val);
	int getFreeByte(DWORD &val);
};

#endif