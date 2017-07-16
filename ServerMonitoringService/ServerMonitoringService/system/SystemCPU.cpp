#include "SystemCPU.hpp"

TCHAR* SystemCpu::getCpuModeInfo()
{
    HKEY key;
    TCHAR data[1024];
	DWORD dwSize;
	dwSize = sizeof(data) / sizeof(TCHAR);

	if (RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"), &key) != ERROR_SUCCESS)
		throw -1;    

	if (RegQueryValueEx(key, TEXT("ProcessorNameString"), NULL, NULL, (LPBYTE)data, &dwSize) != ERROR_SUCCESS)
		throw -2;

    RegCloseKey(key);

	return data;
}

int SystemCpu::getUsage(double &val)
{
	FILETIME sysIdle, sysKernel, sysUser;
	// sysKernel include IdleTime
	if (GetSystemTimes(&sysIdle, &sysKernel, &sysUser) == 0) // GetSystemTimes func FAILED return value is zero;
		return 0;

	if (prevSysIdle.dwLowDateTime != 0 && prevSysIdle.dwHighDateTime != 0)
	{
		ULONGLONG sysIdleDiff, sysKernelDiff, sysUserDiff;
		sysIdleDiff = SubtractTimes(sysIdle, prevSysIdle);
		sysKernelDiff = SubtractTimes(sysKernel, prevSysKernel);
		sysUserDiff = SubtractTimes(sysUser, prevSysUser);

		ULONGLONG sysTotal = sysKernelDiff + sysUserDiff;
		ULONGLONG kernelTotal = sysKernelDiff - sysIdleDiff; // kernelTime - IdleTime = kernelTime, because sysKernel include IdleTime

		if (sysTotal > 0) // sometimes kernelTime > idleTime
			val = (double)(((kernelTotal + sysUserDiff) * 100.0) / sysTotal);
	}

	prevSysIdle = sysIdle;
	prevSysKernel = sysKernel;
	prevSysUser = sysUser;

	return 1;
}

// TIME DIFF FUNC
ULONGLONG SystemCpu::SubtractTimes(const FILETIME one, const FILETIME two)
{
	LARGE_INTEGER a, b;
	a.LowPart = one.dwLowDateTime;
	a.HighPart = one.dwHighDateTime;

	b.LowPart = two.dwLowDateTime;
	b.HighPart = two.dwHighDateTime;

	return a.QuadPart - b.QuadPart;
}