#ifndef _SYSTEMNETWORK_H_
#define _SYSTEMNETWORK_H_

#include <iostream>
#include <Windows.h>
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")

#define KBPS  8 / 1000
#define MBPS  8 / 1000 / 1000

using namespace std;

class SystemNetwork
{
public:
	int getNetworkInterfaces(PIP_ADAPTER_INFO &pAdapter, int &outCount);
	int getNetworkSendBytes();
	int getNetworkReceiveBytes();

};

#endif