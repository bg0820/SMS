#include "SystemNetwork.hpp"

int main()
{
	ULONG count;
	SystemNetwork sn;
	NetworkConnection *networkConnection;
	sn.getConnectionTable(networkConnection, count);

	for (ULONG i = 0; i < count; i++)
	{
		cout << networkConnection[i].processID << endl;
	}

	system("pause");
}


TCHAR* SystemNetwork::getInterfaceTypeName(IP_ADAPTER_INFO parmAdapterInfo)
{
	switch (parmAdapterInfo.Type) {
	case MIB_IF_TYPE_OTHER:
		return TEXT("Other");
		break;
	case MIB_IF_TYPE_ETHERNET:
		return TEXT("Ethernet");
		break;
	case MIB_IF_TYPE_TOKENRING:
		return TEXT("Token Ring");
		break;
	case MIB_IF_TYPE_FDDI:
		return TEXT("FDDI");
		break;
	case MIB_IF_TYPE_PPP:
		return TEXT("PPP");
		break;
	case MIB_IF_TYPE_LOOPBACK:
		return TEXT("Lookback");
		break;
	case MIB_IF_TYPE_SLIP:
		return TEXT("Slip");
		break;
	case IF_TYPE_ATM:
		return TEXT("ATM");
		break;
	case IF_TYPE_IEEE80211:
		return TEXT("IEEE 802.11 Wireless");
		break;
	case IF_TYPE_TUNNEL:
		return TEXT("Tunnel type encapsulation");
		break;
	case IF_TYPE_IEEE1394:
		return TEXT("IEEE 1394 Firewire");
		break;
	default:
		return TEXT("Unknown type");
		break;
	}
}


TCHAR* SystemNetwork::getProtocolTypeName(ULONG protocolType)
{
	switch (protocolType)
	{
	case TCP4_NETWORK_PROTOCOL:
		return TEXT("TCP");
	case TCP6_NETWORK_PROTOCOL:
		return TEXT("TCP6");
	case UDP4_NETWORK_PROTOCOL:
		return TEXT("UDP");
	case UDP6_NETWORK_PROTOCOL:
		return TEXT("UDP6");
	default:
		return TEXT("Unknown");
	}
}

TCHAR * SystemNetwork::getStateName(ULONG state)
{
	switch (state)
	{
	case MIB_TCP_STATE_CLOSED:
		return TEXT("Closed");
	case MIB_TCP_STATE_LISTEN:
		return TEXT("Listen");
	case MIB_TCP_STATE_SYN_SENT:
		return TEXT("SYN Sent");
	case MIB_TCP_STATE_SYN_RCVD:
		return TEXT("SYN Received");
	case MIB_TCP_STATE_ESTAB:
		return TEXT("Established");
	case MIB_TCP_STATE_FIN_WAIT1:
		return TEXT("FIN Wait 1");
	case MIB_TCP_STATE_FIN_WAIT2:
		return TEXT("FIN Wait 2");
	case MIB_TCP_STATE_CLOSE_WAIT:
		return TEXT("Close Wait");
	case MIB_TCP_STATE_CLOSING:
		return TEXT("Closing");
	case MIB_TCP_STATE_LAST_ACK:
		return TEXT("Last ACK");
	case MIB_TCP_STATE_TIME_WAIT:
		return TEXT("Time Wait");
	case MIB_TCP_STATE_DELETE_TCB:
		return TEXT("Delete TCB");
	default:
		return TEXT("Unknown");
	}
}

int SystemNetwork::getConnectionTable(NetworkConnection *&parmConnection, ULONG &parmCount)
{
	int nResult = 1;

	int totalCount = 0;
	int index = 0;
	DWORD dwSize;
	PVOID table;
	MIB_TCPTABLE_OWNER_MODULE *tcp4Table = NULL;
	MIB_UDPTABLE_OWNER_MODULE *udp4Table = NULL;
	MIB_TCP6TABLE_OWNER_MODULE *tcp6Table = NULL;
	MIB_UDP6TABLE_OWNER_MODULE *udp6Table = NULL;
	NetworkConnection *connections;

	//PMIB_TCP6TABLE tcp6Table;
	//PMIB_UDP6TABLE udp6Table;
	// TCP IPv4
	dwSize = 0;
	GetExtendedTcpTable(NULL, &dwSize, FALSE, AF_INET, TCP_TABLE_OWNER_MODULE_ALL, 0);

	table = new PVOID[dwSize / sizeof(PVOID)];
	//table = new PVOID[dwSize]; // size; dwSize / sizeof(PVOID) ?

	if (GetExtendedTcpTable(table, &dwSize, FALSE, AF_INET, TCP_TABLE_OWNER_MODULE_ALL, 0) == 0)
	{
		tcp4Table = (PMIB_TCPTABLE_OWNER_MODULE)table;
		totalCount += tcp4Table->dwNumEntries;
	}
	else
	{
		if (table)
		{
			delete table;
			table = NULL;
		}
		tcp4Table = NULL;

		nResult = 0;
	}

	// TCP IPv6
	dwSize = 0;
	GetExtendedTcpTable(NULL, &dwSize, FALSE, 23, TCP_TABLE_OWNER_MODULE_ALL, 0);

	table = new PVOID[dwSize / sizeof(PVOID)];

	if (GetExtendedTcpTable(table, &dwSize, FALSE, 23, TCP_TABLE_OWNER_MODULE_ALL, 0) == 0)
	{
		tcp6Table = (PMIB_TCP6TABLE_OWNER_MODULE)table;
		totalCount += tcp6Table->dwNumEntries;
	}
	else
	{
		if (table)
		{
			delete table;
			table = NULL;
		}
		tcp6Table = NULL;

		nResult = 0;
	}

	// UDP IPv4
	dwSize = 0;
	GetExtendedUdpTable(NULL, &dwSize, FALSE, AF_INET, UDP_TABLE_OWNER_MODULE, 0);

	table = new PVOID[dwSize / sizeof(PVOID)];

	if (GetExtendedUdpTable(table, &dwSize, FALSE, AF_INET, UDP_TABLE_OWNER_MODULE, 0) == 0)
	{
		udp4Table = (PMIB_UDPTABLE_OWNER_MODULE)table;
		totalCount += udp4Table->dwNumEntries;
	}
	else
	{
		if (table)
		{
			delete table;
			table = NULL;
		}
		udp4Table = NULL;

		nResult = 0;
	}

	// UDP IPv6
	dwSize = 0;
	GetExtendedUdpTable(NULL, &dwSize, FALSE, 23, UDP_TABLE_OWNER_MODULE, 0);

	table = new PVOID[dwSize / sizeof(PVOID)];

	if (GetExtendedUdpTable(table, &dwSize, FALSE, 23, UDP_TABLE_OWNER_MODULE, 0) == 0)
	{
		udp6Table = (PMIB_UDP6TABLE_OWNER_MODULE)table;
		totalCount += udp6Table->dwNumEntries;
	}
	else
	{
		if (table)
		{
			delete table;
			table = NULL;
		}
		udp6Table = NULL;

		nResult = 0;
	}

	connections = new NetworkConnection[totalCount];

	if (tcp4Table)
	{
		for (int i = 0; i < tcp4Table->dwNumEntries; i++)
		{
			connections[index].protocolType = TCP4_NETWORK_PROTOCOL;
			connections[index].localEndpoint.Address.Type = IPv4_NETWORK_TYPE;
			connections[index].localEndpoint.Address.Ipv4 = tcp4Table->table[i].dwLocalAddr;
			connections[index].localEndpoint.Port = (USHORT)tcp4Table->table[i].dwLocalPort;
			connections[index].remoteEndpoint.Address.Type = IPv4_NETWORK_TYPE;
			connections[index].remoteEndpoint.Address.Ipv4 = tcp4Table->table[i].dwRemoteAddr;
			connections[index].remoteEndpoint.Port = (USHORT)tcp4Table->table[i].dwRemotePort;
			connections[index].state = tcp4Table->table[i].dwState;
			connections[index].processID = tcp4Table->table[i].dwOwningPid;
			connections[index].createTime = tcp4Table->table[i].liCreateTimestamp;
			memcpy(connections[index].ownerInfo, tcp4Table->table[i].OwningModuleInfo, sizeof(ULONGLONG) *NETWORK_OWNER_INFO_SIZE);
			index++;
		}

		delete tcp4Table;
		tcp4Table = nullptr;
	}
	else
		nResult = 0;

	if (tcp6Table)
	{
		for (int i = 0; i < tcp6Table->dwNumEntries; i++)
		{
			connections[index].protocolType = TCP6_NETWORK_PROTOCOL;
			connections[index].localEndpoint.Address.Type = IPv6_NETWORK_TYPE;
			memcpy(connections[index].localEndpoint.Address.Ipv6, tcp6Table->table[i].ucLocalAddr, 16);
			connections[index].localEndpoint.Port = (USHORT)tcp6Table->table[i].dwLocalPort;
			connections[index].remoteEndpoint.Address.Type = IPv6_NETWORK_TYPE;
			memcpy(connections[index].remoteEndpoint.Address.Ipv6, tcp6Table->table[i].ucRemoteAddr, 16);
			connections[index].remoteEndpoint.Port = (USHORT)tcp6Table->table[i].dwRemotePort;
			connections[index].state = tcp6Table->table[i].dwState;
			connections[index].processID = tcp6Table->table[i].dwOwningPid;
			connections[index].createTime = tcp6Table->table[i].liCreateTimestamp;
			memcpy(connections[index].ownerInfo, tcp6Table->table[i].OwningModuleInfo, sizeof(ULONGLONG) *NETWORK_OWNER_INFO_SIZE);
			index++;
		}

		delete tcp6Table;
		tcp6Table = nullptr;
	}
	else
		nResult = 0;

	if (udp4Table)
	{
		for (int i = 0; i < udp4Table->dwNumEntries; i++)
		{
			connections[index].protocolType = UDP4_NETWORK_PROTOCOL;
			connections[index].localEndpoint.Address.Type = IPv4_NETWORK_TYPE;
			connections[index].localEndpoint.Address.Ipv4 = udp4Table->table[i].dwLocalAddr;
			connections[index].localEndpoint.Port = (USHORT)udp4Table->table[i].dwLocalPort;
			connections[index].remoteEndpoint.Address.Type = 0;
			connections[index].state = 0;
			connections[index].processID = udp4Table->table[i].dwOwningPid;
			connections[index].createTime = udp4Table->table[i].liCreateTimestamp;
			memcpy(connections[index].ownerInfo, udp4Table->table[i].OwningModuleInfo, sizeof(ULONGLONG) *NETWORK_OWNER_INFO_SIZE);
			index++;
		}

		delete udp4Table;
		udp4Table = nullptr;
	}
	else
		nResult = 0;

	if (udp6Table)
	{
		cout << udp6Table->dwNumEntries << endl;
		for (int i = 0; i < udp6Table->dwNumEntries; i++)
		{
			connections[index].protocolType = UDP6_NETWORK_PROTOCOL;
			connections[index].localEndpoint.Address.Type = IPv6_NETWORK_TYPE;
			memcpy(connections[index].localEndpoint.Address.Ipv6, udp6Table->table[i].ucLocalAddr, 16);
			connections[index].localEndpoint.Port = (USHORT)udp6Table->table[i].dwLocalPort;
			connections[index].remoteEndpoint.Address.Type = 0;
			connections[index].state = 0;
			connections[index].processID = udp6Table->table[i].dwOwningPid;
			connections[index].createTime = udp6Table->table[i].liCreateTimestamp;
			memcpy(connections[index].ownerInfo, udp6Table->table[i].OwningModuleInfo, sizeof(ULONGLONG) * NETWORK_OWNER_INFO_SIZE);
			index++;
		}

		delete udp6Table;
		udp6Table = nullptr;
	}
	else
		nResult = 0;

	parmConnection = connections;
	parmCount = totalCount;

	return nResult;
}

int SystemNetwork::getNetworkSendBytes()
{
	//Win32_PerfFormattedData_Tcpip_NetworkInterface win;
	return 1;
}

// you should parm PIP_ADAPTER_INFO obj delete
int SystemNetwork::getInterfaces(PIP_ADAPTER_INFO &parmAdapter, int &outCount)
{
	int nResult = 1;

	PIP_ADAPTER_INFO pAdapterInfo = NULL;
	DWORD dwRetVal = 0;

	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	pAdapterInfo = new IP_ADAPTER_INFO; // 1 size

	if (pAdapterInfo == NULL) // printf("Error allocating memory needed to call GetAdaptersinfo\n");
		nResult = -1;

	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen))
	{
		delete pAdapterInfo;
		pAdapterInfo = nullptr;

		outCount = ulOutBufLen / sizeof(IP_ADAPTER_INFO);
		pAdapterInfo = new IP_ADAPTER_INFO[outCount]; // because (IP_ADAPTER_INFO *) MALLOC(ulOutBufLen);

		if (pAdapterInfo == NULL) // printf("Error allocating memory needed to call GetAdaptersinfo\n");
			nResult = -2;
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)))
	{
		parmAdapter = pAdapterInfo;
	}
	else
		nResult = dwRetVal; // set return value

	return nResult;
}

void SystemNetwork::formatToMacAddress(TCHAR *parm, BYTE addr[])
{
	//CString strMacAddr = "";
	//strMacAddr.format("%02x%02x%02x%02x%02x%02x",
	//	parmAdapter->Address[0],
	//	parmAdapter->Address[1],
	//	parmAdapter->Address[2],
	//	parmAdapter->Address[3],
	//	parmAdapter->Address[4],
	//	parmAdapter->Address[5]);

	sprintf(parm, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",
		addr[0],
		addr[1],
		addr[2],
		addr[3],
		addr[4],
		addr[5]);
}

/*
int main()
{
	int count = 0;
	PIP_ADAPTER_INFO parmAdapter; // delete
	SystemNetwork net;


	if (net.getNetworkInterfaces(parmAdapter, count))
	{

		for (int k = 0; k < count; k++)
		{
			printf("\tComboIndex: \t%d\n", parmAdapter[k].ComboIndex);
			printf("\tAdapter Name: \t%s\n", parmAdapter[k].AdapterName);
			printf("\tAdapter Desc: \t%s\n", parmAdapter[k].Description);
			printf("\tAdapter Addr: \t");

			TCHAR *macAddr = new TCHAR[18];
			net.formatToMacAddress(macAddr, parmAdapter[k].Address);
			// TODO change
			cout << macAddr << endl;

			string strType = net.getNetworkInterfaceType(parmAdapter[k]);
		}
	}

	if (parmAdapter)
	{
		delete[] parmAdapter;
		parmAdapater = nullptr;
	}
	system("pause");
}*/

/*
#define _WIN32_DCOM

#include "Process.hpp"
#include <iostream>
using namespace std;
#include <Wbemidl.h>


#pragma comment(lib, "wbemuuid.lib")

int __cdecl wmain(int argc, wchar_t* argv[])
{
	// To add error checking,
	// check returned HRESULT below where collected.
	HRESULT                 hr = S_OK;
	IWbemRefresher          *pRefresher = NULL;
	IWbemConfigureRefresher *pConfig = NULL;
	IWbemHiPerfEnum         *pEnum = NULL;
	IWbemServices           *pNameSpace = NULL;
	IWbemLocator            *pWbemLocator = NULL;
	IWbemObjectAccess       **apEnumAccess = NULL;
	BSTR                    bstrNameSpace = NULL;
	long                    lID = 0;
	long                    lVirtualBytesHandle = 0;
	long                    lIDProcessHandle = 0;
	DWORD                   dwVirtualBytes = 0;
	DWORD                   dwProcessId = 0;
	DWORD                   dwNumObjects = 0;
	DWORD                   dwNumReturned = 0;
	DWORD                   dwIDProcess = 0;
	DWORD                   i = 0;
	int                     x = 0;

	if (FAILED(hr = CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	{
		goto CLEANUP;
	}

	if (FAILED(hr = CoInitializeSecurity(
		NULL,
		-1,
		NULL,
		NULL,
		RPC_C_AUTHN_LEVEL_NONE,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL, EOAC_NONE, 0)))
	{
		goto CLEANUP;
	}

	if (FAILED(hr = CoCreateInstance(
		CLSID_WbemLocator,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator,
		(void**)&pWbemLocator)))
	{
		goto CLEANUP;
	}

	// Connect to the desired namespace.
	bstrNameSpace = SysAllocString(L"\\\\.\\root\\cimv2");
	if (NULL == bstrNameSpace)
	{
		hr = E_OUTOFMEMORY;
		goto CLEANUP;
	}
	if (FAILED(hr = pWbemLocator->ConnectServer(
		bstrNameSpace,
		NULL, // User name
		NULL, // Password
		NULL, // Locale
		0L,   // Security flags
		NULL, // Authority
		NULL, // Wbem context
		&pNameSpace)))
	{
		goto CLEANUP;
	}
	pWbemLocator->Release();
	pWbemLocator = NULL;
	SysFreeString(bstrNameSpace);
	bstrNameSpace = NULL;

	if (FAILED(hr = CoCreateInstance(
		CLSID_WbemRefresher,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWbemRefresher,
		(void**)&pRefresher)))
	{
		goto CLEANUP;
	}

	if (FAILED(hr = pRefresher->QueryInterface(
		IID_IWbemConfigureRefresher,
		(void **)&pConfig)))
	{
		goto CLEANUP;
	}

	// Add an enumerator to the refresher.
	if (FAILED(hr = pConfig->AddEnum(
		pNameSpace,
		L"Win32_PerfRawData_PerfProc_Process", // Win32_PerfRawData_PerfProc_Process
		0,
		NULL,
		&pEnum,
		&lID)))
	{
		goto CLEANUP;
	}
	pConfig->Release();
	pConfig = NULL;

	// Get a property handle for the VirtualBytes property.

	// Refresh the object ten times and retrieve the value.
	while(true)
	{
		dwNumReturned = 0;
		dwIDProcess = 0;
		dwNumObjects = 0;

		if (FAILED(hr = pRefresher->Refresh(0L)))
		{
			goto CLEANUP;
		}

		hr = pEnum->GetObjects(0L,
			dwNumObjects,
			apEnumAccess,
			&dwNumReturned);
		// If the buffer was not big enough,
		// allocate a bigger buffer and retry.
		if (hr == WBEM_E_BUFFER_TOO_SMALL
			&& dwNumReturned > dwNumObjects)
		{
			apEnumAccess = new IWbemObjectAccess*[dwNumReturned];
			if (NULL == apEnumAccess)
			{
				hr = E_OUTOFMEMORY;
				goto CLEANUP;
			}
			SecureZeroMemory(apEnumAccess,
				dwNumReturned * sizeof(IWbemObjectAccess*));
			dwNumObjects = dwNumReturned;

			if (FAILED(hr = pEnum->GetObjects(0L,
				dwNumObjects,
				apEnumAccess,
				&dwNumReturned)))
			{
				goto CLEANUP;
			}
		}
		else
		{
			if (hr == WBEM_S_NO_ERROR)
			{
				hr = WBEM_E_NOT_FOUND;
				goto CLEANUP;
			}
		}

		// First time through, get the handles.
		if (0 == x)
		{
			CIMTYPE VirtualBytesType;
			CIMTYPE ProcessHandleType;
			if (FAILED(hr = apEnumAccess[0]->GetPropertyHandle(
				L"IOWriteBytesPerSec",
				&VirtualBytesType,
				&lVirtualBytesHandle)))
			{
				goto CLEANUP;
			}
			if (FAILED(hr = apEnumAccess[0]->GetPropertyHandle(
				L"IDProcess",
				&ProcessHandleType,
				&lIDProcessHandle)))
			{
				goto CLEANUP;
			}
		}

		for (i = 0; i < dwNumReturned; i++)
		{
			if (FAILED(hr = apEnumAccess[i]->ReadDWORD(
				lVirtualBytesHandle,
				&dwVirtualBytes)))
			{
				goto CLEANUP;
			}
			if (FAILED(hr = apEnumAccess[i]->ReadDWORD(
				lIDProcessHandle,
				&dwIDProcess)))
			{
				goto CLEANUP;
			}

			Process process = Process(dwIDProcess);
			TCHAR* tc = process.getName();

			if(dwIDProcess == 20620)
				printf("Process ID %lu %s is using %.2f Mbyte\n", dwIDProcess, tc, dwVirtualBytes / 1024.0 / 1024.0);
			//wprintf(L"Process ID %lu %s is using %lu bytes\n",			dwIDProcess, tc,  dwVirtualBytes);

			// Done with the object
			apEnumAccess[i]->Release();
			apEnumAccess[i] = NULL;
		}

		if (NULL != apEnumAccess)
		{
			delete[] apEnumAccess;
			apEnumAccess = NULL;
		}

		// Sleep for a second.
		Sleep(1000);
	}
	// exit loop here
CLEANUP:

	if (NULL != bstrNameSpace)
	{
		SysFreeString(bstrNameSpace);
	}

	if (NULL != apEnumAccess)
	{
		for (i = 0; i < dwNumReturned; i++)
		{
			if (apEnumAccess[i] != NULL)
			{
				apEnumAccess[i]->Release();
				apEnumAccess[i] = NULL;
			}
		}
		delete[] apEnumAccess;
	}
	if (NULL != pWbemLocator)
	{
		pWbemLocator->Release();
	}
	if (NULL != pNameSpace)
	{
		pNameSpace->Release();
	}
	if (NULL != pEnum)
	{
		pEnum->Release();
	}
	if (NULL != pConfig)
	{
		pConfig->Release();
	}
	if (NULL != pRefresher)
	{
		pRefresher->Release();
	}

	CoUninitialize();

	if (FAILED(hr))
	{
		wprintf(L"Error status=%08x\n", hr);
	}

	system("pause");
	return 1;
}*/