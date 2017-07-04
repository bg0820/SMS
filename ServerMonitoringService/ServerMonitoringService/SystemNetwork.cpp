#include "SystemNetwork.hpp"

string SystemNetwork::getNetworkInterfaceType(IP_ADAPTER_INFO parmAdapterInfo)
{
	string strType = NULL;

	switch (parmAdapterInfo.Type) {
	case MIB_IF_TYPE_OTHER:
		strType = "Other";
		break;
	case MIB_IF_TYPE_ETHERNET:
		strType = "Ethernet";
		break;
	case MIB_IF_TYPE_TOKENRING:
		strType = "Token Ring";
		break;
	case MIB_IF_TYPE_FDDI:
		strType = "FDDI";
		break;
	case MIB_IF_TYPE_PPP:
		strType = "PPP";
		break;
	case MIB_IF_TYPE_LOOPBACK:
		strType = "Lookback";
		break;
	case MIB_IF_TYPE_SLIP:
		strType = "Slip";
		break;
	case IF_TYPE_ATM:
		strType = "ATM";
		break;
	case IF_TYPE_IEEE80211:
		strType = "IEEE 802.11 Wireless";
		break;
	case IF_TYPE_TUNNEL:
		strType = "Tunnel type encapsulation";
		break;
	case IF_TYPE_IEEE1394:
		strType = "IEEE 1394 Firewire";
		break;
	default:
		strType = "Unknown type";
		break;
	}

	return strType;
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
		pAdapterInfo = nullptr;

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