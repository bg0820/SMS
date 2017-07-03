#ifndef _SYSTEMCPU_H_
#define _SYSTEMCPU_H_

#include <Windows.h>

typedef unsigned __int64 ULONGLONG;

class SystemCpu
{
private:
	FILETIME prevSysIdle, prevSysKernel, prevSysUser;
	ULONGLONG SubtractTimes(FILETIME one, FILETIME two);
public:
	int getUsage(double &val);
	int getIdleUsage(double &val);
	TCHAR* getCpuModeInfo();
	int getCpuHz(int &val);
	//int getSysUsage(double &val);
};

#endif
