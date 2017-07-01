#include "SystemOS.h"

// you need to delete char *parm 
int SystemOS::getUserName(char *& val)
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
int SystemOS::getComputerName(char *& val)
{
	TCHAR charBuf[BUFFER_SIZE];
	DWORD bufCharSize = BUFFER_SIZE;

	if (!GetComputerName(charBuf, &bufCharSize)) // GetComputerName Func Failed return value is ZERO
		return 0;

	val = new TCHAR[bufCharSize];
	memcpy(val, charBuf, bufCharSize);

	return 1;
}
