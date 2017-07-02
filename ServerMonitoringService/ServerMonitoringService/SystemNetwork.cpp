#include "SystemNetwork.hpp"

int SystemNetwork::getNetworkInterfaceType(int type)
{
	switch (type) {
	case MIB_IF_TYPE_OTHER:
		printf("Other\n");
		break;
	case MIB_IF_TYPE_ETHERNET:
		printf("Ethernet\n");
		break;
	case MIB_IF_TYPE_TOKENRING:
		printf("Token Ring\n");
		break;
	case MIB_IF_TYPE_FDDI:
		printf("FDDI\n");
		break;
	case MIB_IF_TYPE_PPP:
		printf("PPP\n");
		break;
	case MIB_IF_TYPE_LOOPBACK:
		printf("Lookback\n");
		break;
	case MIB_IF_TYPE_SLIP:
		printf("Slip\n");
		break;
	case IF_TYPE_ATM:
		printf("ATM\n");
		break;
	case IF_TYPE_IEEE80211:
		printf("IEEE 802.11 Wireless\n");
		break;
	case IF_TYPE_TUNNEL:
		printf("Tunnel type encapsulation\n");
		break;
	case IF_TYPE_IEEE1394:
		printf("IEEE 1394 Firewire\n");
		break;
	default:
		printf("Unknown type %ld\n", type);
		break;
	}

	return 1;
}

int SystemNetwork::getNetworkSendBytes()
{
	//Win32_PerfFormattedData_Tcpip_NetworkInterface win;
	return 1;
}

// you should parm PIP_ADAPTER_INFO obj delete
int SystemNetwork::getNetworkInterfaces(PIP_ADAPTER_INFO &parmAdapter, int &outCount)
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
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		delete pAdapterInfo;
		pAdapterInfo = NULL;

		outCount = ulOutBufLen / sizeof(IP_ADAPTER_INFO);
		pAdapterInfo = new IP_ADAPTER_INFO[outCount]; // because (IP_ADAPTER_INFO *) MALLOC(ulOutBufLen);

		if (pAdapterInfo == NULL) // printf("Error allocating memory needed to call GetAdaptersinfo\n");
			nResult = -2;
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
	{
		parmAdapter = pAdapterInfo;
	}
	else
		nResult = dwRetVal; // set return value

	return nResult;
}

void SystemNetwork::formatToMacAddress(TCHAR *parm, BYTE addr[])
{
	sprintf(parm, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",
		addr[0],
		addr[1],
		addr[2],
		addr[3],
		addr[4],
		addr[5]);
}

//CString strMacAddr = "";
//strMacAddr.format("%02x%02x%02x%02x%02x%02x",
//	parmAdapter->Address[0],
//	parmAdapter->Address[1],
//	parmAdapter->Address[2],
//	parmAdapter->Address[3],
//	parmAdapter->Address[4],
//	parmAdapter->Address[5]);

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
			
			printf("\tIndex: \t%d\n", parmAdapter[k].Index);
			printf("\tType: \t");
			net.getNetworkInterfaceType(parmAdapter[k].Type);


			printf("\tIP Address: \t%s\n",
				parmAdapter[k].IpAddressList.IpAddress.String);
			printf("\tIP Mask: \t%s\n", parmAdapter[k].IpAddressList.IpMask.String);

			printf("\tGateway: \t%s\n", parmAdapter[k].GatewayList.IpAddress.String);

			printf("\n");
		}
	}

	if (parmAdapter)
		delete parmAdapter;

	system("pause");
}