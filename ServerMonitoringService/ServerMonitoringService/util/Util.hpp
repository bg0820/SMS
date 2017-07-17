#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <time.h>
#include <chrono>
#include <memory>
/*
LZ4 Compress Library
*/
#include "../lib/lz4/lz4.h"

using namespace std;
using namespace chrono;
class Util
{
public:
	static string currentDateTime();
	static string format(const string fmt_str, ...);
	static string getLocalIp();
	static string getStringToAddr(SOCKADDR_IN paramAddr);
	static void stringReplace(string& subject, const string& search, const string& replace);
	static void microDelay(ULONG time);
	static string getTimeString(tm t);
	static int Compress(const TCHAR *paramRawData, int &paramSrcSize, TCHAR *&paramCompressedData, int &paramCompressSize);
	static int DeCompress(const TCHAR *paramCompressedData, int paramCompressedDataSize, int paramRawSize, TCHAR *&paramRawData);
};

#endif