#ifndef _SYSTEMNETWORK_H_
#define _SYSTEMNETWORK_H_

#include <iostream>
#include <Windows.h>

#include <ws2tcpip.h>
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "ws2_32.lib") // WinSock2 Lib

#define KBPS  8 / 1000
#define MBPS  8 / 1000 / 1000
#define IPv4_NETWORK_TYPE 0x1
#define IPv6_NETWORK_TYPE 0x2
#define TCP_PROTOCOL_TYPE 0x10
#define UDP_PROTOCOL_TYPE 0x20
#define TCP4_NETWORK_PROTOCOL (IPv4_NETWORK_TYPE | TCP_PROTOCOL_TYPE)
#define TCP6_NETWORK_PROTOCOL (IPv6_NETWORK_TYPE | TCP_PROTOCOL_TYPE)
#define UDP4_NETWORK_PROTOCOL (IPv4_NETWORK_TYPE | UDP_PROTOCOL_TYPE)
#define UDP6_NETWORK_PROTOCOL (IPv6_NETWORK_TYPE | UDP_PROTOCOL_TYPE)
#define NETWORK_OWNER_INFO_SIZE 16

using namespace std;

typedef struct ip_address
{
	ULONG Type;
	union
	{
		ULONG Ipv4;
		struct in_addr InAddr;
		UCHAR Ipv6[16];
		struct in6_addr In6Addr;
	};
} IP_ADDRESS;

typedef struct ip_endpoint
{
	IP_ADDRESS Address;
	ULONG Port;
} IP_ENDPOINT;

typedef struct network_connection
{
	ULONG protocolType;
	IP_ENDPOINT localEndpoint;
	IP_ENDPOINT remoteEndpoint;
	ULONG state;
	HANDLE processID;
	LARGE_INTEGER createTime;
	ULONGLONG ownerInfo[NETWORK_OWNER_INFO_SIZE];
}NetworkConnection;

class SystemNetwork
{
public:
	void formatToMacAddress(TCHAR *parm, BYTE addr[]);
	int getNetworkInterfaces(PIP_ADAPTER_INFO &pAdapter, int &outCount);
	string getNetworkInterfaceType(IP_ADAPTER_INFO parmAdapterInfo);
	int getNetworkTable(NetworkConnection *&parmConnection, ULONG &parmCount);
	int getNetworkSendBytes();
	int getNetworkReceiveBytes();
};

#endif