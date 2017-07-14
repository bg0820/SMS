#include "main.hpp"

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
	DataManager dataManager = DataManager();
	// SystemInfo* systemInfo = dataManager.getSystem();

	system("pause");
}

/*
int main(int argc, char* argv[])
{
	// 메모리 누수 디버깅
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// 메모리 누수 찾기
	// _CrtSetBreakAlloc(165);
#endif


	if (IsUserAnAdmin())
	{
		mainStart();
	}
	else
	{
		onShellExecute();
	}

	return 0;
}*/
