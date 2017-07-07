#include <iostream>
#include <windows.h>	
#include <Shlobj.h>
#include "system/SystemNetwork.hpp"

#define DIV				1024						// KB
#define DIV_TWO			1024 / 1024					// MB
#define DIV_THREE		1024 / 1024 / 1024			// GB


using namespace std;

void onShellExecute()
{
	SHELLEXECUTEINFO shExeInfo;
	TCHAR strPath[MAX_PATH];

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
	
}


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
