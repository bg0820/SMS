#ifndef _STDAFX_H_
#define _STDAFX_H_

#include <cstdarg>
#include <memory>
#include <functional>
#include <chrono>
#include <tchar.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <string>
#include <vector>

#ifndef _WINDOWS_
#include <ws2tcpip.h>
#include <windows.h>
#endif

// Process.hpp
#include <shlobj.h>
#include <shlguid.h>
#include <shellapi.h>
#include <commctrl.h>
#include <commoncontrols.h>
#include <tlhelp32.h>
#include <Winternl.h>

#include <conio.h>
#include <WinSvc.h>

#include <Psapi.h>
#include <iphlpapi.h>

#pragma comment(lib, "ws2_32.lib") // WinSock2 Lib
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "psapi.lib")

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#endif
