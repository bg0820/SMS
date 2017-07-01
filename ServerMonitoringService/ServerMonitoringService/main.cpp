#include <iostream>
#include <windows.h>	
#include <Shlobj.h>
#include "SystemNetwork.hpp"

#define DIV				1024						// KB
#define DIV_TWO			1024 / 1024					// MB
#define DIV_THREE		1024 / 1024 / 1024			// GB


using namespace std;

void onShellExecute()
{
	SHELLEXECUTEINFO shExeInfo;
	char strPath[MAX_PATH] = "";

	GetModuleFileName(NULL, strPath, MAX_PATH);

	shExeInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	shExeInfo.fMask = NULL;
	shExeInfo.hwnd = NULL;
	shExeInfo.lpVerb = TEXT("runas");
	shExeInfo.lpFile = strPath;
	shExeInfo.lpParameters = NULL;
	shExeInfo.lpDirectory = NULL;
	shExeInfo.nShow = SW_NORMAL;
	shExeInfo.hInstApp = NULL;

	if (!ShellExecuteEx(&shExeInfo))
	{
		DWORD dwStatus = GetLastError();
		if (dwStatus == ERROR_CANCELLED)
		{
			// TODO : Admin Permission Denied
		}
		// else if (dwStatus == ERROR_FILE_NOT_FOUND) { // TODO : lpFile is not founded; }
	}
}

void mainStart()
{
	int count = 0;
	PIP_ADAPTER_INFO parmAdapter = NULL; // delete
	SystemNetwork net;
	if (net.getNetworkInterfaces(parmAdapter, count))
	{

		cout << count << endl;
		for (int i = 0; i< count; i++)
		{
			cout << parmAdapter[i].IpAddressList.IpAddress.String << endl;

			printf("\tAdapter Addr: \t");

			for (UINT i = 0; i < parmAdapter[i].AddressLength; i++) {
				if (i == (parmAdapter[i].AddressLength - 1))
					printf("%.2X\n", (int)parmAdapter[i].Address[i]);
				else
					printf("%.2X-", (int)parmAdapter[i].Address[i]);
		}

			switch (parmAdapter[i].Type) {
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
			default:
				printf("Unknown type %ld\n", parmAdapter[i].Type);
				break;
			}
			//parmAdapter = parmAdapter[i].Next;
		}
	}

	//delete parmAdapter;

	system("pause");
}
/*
int main(int argc, char* argv[])
{
	if (IsUserAnAdmin())
	{
		mainStart();
	}
	else
	{
		onShellExecute();
	}

	return 0;
}
*/