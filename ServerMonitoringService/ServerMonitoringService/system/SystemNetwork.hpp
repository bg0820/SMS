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
private:
public:
	void formatToMacAddress(TCHAR *parm, BYTE addr[]);
	int getNetworkInterfaces(PIP_ADAPTER_INFO &pAdapter, int &outCount);
	string getNetworkInterfaceType(IP_ADAPTER_INFO parmAdapterInfo);
	int getNetworkSendBytes();
	int getNetworkReceiveBytes();

};

#endif