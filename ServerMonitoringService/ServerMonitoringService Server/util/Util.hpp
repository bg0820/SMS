#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <string>
#include <Windows.h>
#include <cstdarg>
#include <memory>

using namespace std;

class Util
{
public:
	static string currentDateTime();
	static string string_format(const string fmt_str, ...);
	static string getLocalIp();
};

#endif