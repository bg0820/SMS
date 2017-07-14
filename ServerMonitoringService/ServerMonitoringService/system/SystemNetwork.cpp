#include "SystemNetwork.hpp"

TCHAR* SystemNetwork::getInterfaceTypeName(IP_ADAPTER_INFO paramAdapterInfo)
{
	switch (paramAdapterInfo.Type) {
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

int SystemNetwork::getConnectionTable(NetworkConnection *&paramConnection, ULONG &paramTotalCount)
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
	NetworkConnection *connections = NULL;

	//PMIB_TCP6TABLE tcp6Table;
	//PMIB_UDP6TABLE udp6Table;
	// TCP IPv4n
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

		delete[] tcp4Table;
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

		delete[] tcp6Table;
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

		delete[] udp4Table;
		udp4Table = nullptr;
	}
	else
		nResult = 0;

	if (udp6Table)
	{
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

		delete[] udp6Table;
		udp6Table = nullptr;
	}
	else
		nResult = 0;

	paramConnection = connections;
	paramTotalCount = totalCount;

	return nResult;
}

int SystemNetwork::getNetworkSendBytes()
{
	//Win32_PerfFormattedData_Tcpip_NetworkInterface win;
	return 1;
}

// you should param PIP_ADAPTER_INFO obj delete
int SystemNetwork::getInterfaces(PIP_ADAPTER_INFO &paramAdapter, int &outCount)
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

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == ERROR_SUCCESS)
		paramAdapter = pAdapterInfo;
	else
		nResult = dwRetVal; // set return value

	return nResult;
}

void SystemNetwork::formatToMacAddress(TCHAR *param, BYTE addr[])
{
	string strMacAddr = Util::format(
		param,
		"%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",
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
	PIP_ADAPTER_INFO paramAdapter; // delete
	SystemNetwork net;


	if (net.getNetworkInterfaces(paramAdapter, count))
	{

		for (int k = 0; k < count; k++)
		{
			printf("\tComboIndex: \t%d\n", paramAdapter[k].ComboIndex);
			printf("\tAdapter Name: \t%s\n", paramAdapter[k].AdapterName);
			printf("\tAdapter Desc: \t%s\n", paramAdapter[k].Description);
			printf("\tAdapter Addr: \t");

			TCHAR *macAddr = new TCHAR[18];
			net.formatToMacAddress(macAddr, paramAdapter[k].Address);
			// TODO change
			cout << macAddr << endl;

			string strType = net.getNetworkInterfaceType(paramAdapter[k]);
		}
	}

	if (paramAdapter)
	{
		delete[] paramAdapter;
		paramAdapater = nullptr;
	}
	system("pause");
}*/
