#include "SystemOS.hpp"

// you need to delete char *parm 
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

// you need to delete char *parm 
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

char* SystemOS::getUserName()
{
	return userName;
}

char* SystemOS::getComputerName()
{
	return computerName;
}
