#include "SystemOS.hpp"

/*
	When you are finished with the TCHAR, be sure to delete it. 
*/
int SystemOS::initUserName(TCHAR *&val)
{
	TCHAR charBuf[BUFFER_SIZE];
	DWORD bufCharSize = BUFFER_SIZE;

	if (!GetUserName(charBuf, &bufCharSize)) // GetUserName Func Failed return value is ZERO
		return 0;

	int len = bufCharSize + 1; // Last Null char
	val = new TCHAR[len];
	memset(val, NULL, len);
	strcpy_s(val, len, charBuf);

	return 1;
}

/*
When you are finished with the TCHAR, be sure to delete it.
*/
int SystemOS::initComputerName(TCHAR *&val)
{
	TCHAR charBuf[BUFFER_SIZE];
	DWORD bufCharSize = BUFFER_SIZE;

	if (!GetComputerName(charBuf, &bufCharSize)) // GetComputerName Func Failed return value is ZERO
		return 0;

	int len = bufCharSize + 1; // Last Null char
	val = new TCHAR[len];
	memset(val, NULL, len);
	strcpy_s(val, len, charBuf);

	return 1;
}

/*
When you are finished with the TCHAR, be sure to delete it.
*/
int SystemOS::initOSVersionName(TCHAR *&val)
{
	HKEY key;
	TCHAR data[1024];
	DWORD dwSize;
	dwSize = sizeof(data) / sizeof(TCHAR);

	if (RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion"), &key) != ERROR_SUCCESS)
		return -1;

	if (RegQueryValueEx(key, TEXT("productName"), NULL, NULL, (LPBYTE)data, &dwSize) != ERROR_SUCCESS)
		return -2;

	RegCloseKey(key);

	int len = dwSize + 1; // Last Null char
	val = new TCHAR[len];
	memset(val, NULL, len);
	strcpy_s(val, len, data);

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