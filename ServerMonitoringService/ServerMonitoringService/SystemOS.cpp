#include "SystemOS.hpp"

// When you are finished with the TCHAR, be sure to delete it. 
int SystemOS::initUserName(TCHAR *&val)
{
	TCHAR charBuf[BUFFER_SIZE];
	DWORD bufCharSize = BUFFER_SIZE;

	if (!GetUserName(charBuf, &bufCharSize)) // GetUserName Func Failed return value is ZERO
		return 0;

	val = new TCHAR[bufCharSize];
	memcpy(val, charBuf, bufCharSize);

	return 1;
}

// When you are finished with the TCHAR, be sure to delete it. 
int SystemOS::initComputerName(TCHAR *&val)
{
	TCHAR charBuf[BUFFER_SIZE];
	DWORD bufCharSize = BUFFER_SIZE;

	if (!GetComputerName(charBuf, &bufCharSize)) // GetComputerName Func Failed return value is ZERO
		return 0;

	val = new TCHAR[bufCharSize];
	memcpy(val, charBuf, bufCharSize);

	return 1;
}

// When you are finished with the TCHAR, be sure to delete it. 
int SystemOS::initOSVersionName(TCHAR *&val)
{
	//Software\\Microsoft\\Windows NT\\CurrentVersio
	HKEY key;
	TCHAR data[1024];
	DWORD dwSize;
	dwSize = sizeof(data) / sizeof(TCHAR);

	if (RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion"), &key) != ERROR_SUCCESS)
		return -1;

	if (RegQueryValueEx(key, TEXT("productName"), NULL, NULL, (LPBYTE)data, &dwSize) != ERROR_SUCCESS)
		return -2;

	RegCloseKey(key);

	val = new TCHAR[1024];
	memcpy(val, data, sizeof(data) / sizeof(TCHAR));

	return 1;
}

TCHAR* SystemOS::getUserName()
{
	return SystemOS::userName;
}

TCHAR* SystemOS::getComputerName()
{
	return SystemOS::computerName;
}

TCHAR* SystemOS::getOSVersionName()
{
	return SystemOS::osVersionName;
}