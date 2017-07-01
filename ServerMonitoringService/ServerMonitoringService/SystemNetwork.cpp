#include "SystemNetwork.hpp"

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