#ifndef _LOG_H_
#define _LOG_H_

#include <stdio.h>
#include <Windows.h>

class Log
{
private:
public:
	static void printErrLine(wchar_t *msg, int lineNumber);
	static void errQuit(TCHAR *title, TCHAR *context);
};

#endif