#ifndef _LOG_H_
#define _LOG_H_

#include <stdarg.h>
#include <stdio.h>
#include <Windows.h>
#include "Util.hpp"

class Log
{
private:
public:
	static void printLog(string msg);
	static void printErrLine(string file, int lineNumber, string msg);
	static void errQuit(string title, string context);
};

#endif