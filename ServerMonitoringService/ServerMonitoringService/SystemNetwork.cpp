#include "SystemNetwork.h"

int SystemNetwork::getNetworkSendBytes()
{
	//Win32_PerfFormattedData_Tcpip_NetworkInterface win;
	return 1;
}

int SystemNetwork::getNetworkInterfaces(PIP_ADAPTER_INFO &parmAdapter, int &count)
{
	int nResult = 1;

	PIP_ADAPTER_INFO pAdapterInfo = NULL;
	DWORD dwRetVal = 0;

	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	pAdapterInfo = new IP_ADAPTER_INFO;


	if (pAdapterInfo == NULL) // printf("Error allocating memory needed to call GetAdaptersinfo\n");
		nResult = -1;

	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) != ERROR_SUCCESS)
	{
		delete pAdapterInfo;
		pAdapterInfo = new IP_ADAPTER_INFO[ulOutBufLen];
		if (pAdapterInfo == NULL) // printf("Error allocating memory needed to call GetAdaptersinfo\n");
			nResult = -2;
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
	{
		count = ulOutBufLen / sizeof(IP_ADAPTER_INFO);
		parmAdapter = pAdapterInfo;
	}
	else
		nResult = dwRetVal;

	if (pAdapterInfo)
		delete pAdapterInfo;

	return nResult;
}


//int main()
//{
//	int count = 0;
//	PIP_ADAPTER_INFO parmAdapter = NULL; // delete
//	SystemNetwork net;
//	if (net.getNetworkInterfaces(parmAdapter, count))
//	{
//
//		cout << count << endl;
//		for (int i = 0; i< count; i++)
//		{
//			cout << parmAdapter[i].IpAddressList.IpAddress.String << endl;
//
//			printf("\tAdapter Addr: \t");
//
//			for (UINT i = 0; i < parmAdapter[i].AddressLength; i++) {
//				if (i == (parmAdapter[i].AddressLength - 1))
//					printf("%.2X\n", (int)parmAdapter[i].Address[i]);
//				else
//					printf("%.2X-", (int)parmAdapter[i].Address[i]);
//			}
//
//			switch (parmAdapter[i].Type) {
//			case MIB_IF_TYPE_OTHER:
//				printf("Other\n");
//				break;
//			case MIB_IF_TYPE_ETHERNET:
//				printf("Ethernet\n");
//				break;
//			case MIB_IF_TYPE_TOKENRING:
//				printf("Token Ring\n");
//				break;
//			case MIB_IF_TYPE_FDDI:
//				printf("FDDI\n");
//				break;
//			case MIB_IF_TYPE_PPP:
//				printf("PPP\n");
//				break;
//			case MIB_IF_TYPE_LOOPBACK:
//				printf("Lookback\n");
//				break;
//			case MIB_IF_TYPE_SLIP:
//				printf("Slip\n");
//				break;
//			default:
//				printf("Unknown type %ld\n", parmAdapter[i].Type);
//				break;
//			}
//			//parmAdapter = parmAdapter[i].Next;
//		}
//	}
//
//	//delete parmAdapter;
//
//	system("pause");
//	return 0;
//}