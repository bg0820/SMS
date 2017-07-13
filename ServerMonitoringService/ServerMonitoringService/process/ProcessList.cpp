#include "ProcessList.hpp"

HANDLE GetProcessByName(PCSTR name)
{
	DWORD pid = 0;

	// Create toolhelp snapshot.
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 process;
	ZeroMemory(&process, sizeof(process));
	process.dwSize = sizeof(process);

	// Walkthrough all processes.
	if (Process32First(snapshot, &process))
	{
		do
		{
			// Compare process.szExeFile based on format of name, i.e., trim file path
			// trim .exe if necessary, etc.
			cout<<process. << endl;
			if (string(process.szExeFile) == string(name))
			{
				pid = process.th32ProcessID;
				break;
			}
		} while (Process32Next(snapshot, &process));
	}

	CloseHandle(snapshot);

	if (pid != 0)
	{
		return OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	}

	// Not found


	return NULL;
}

int main()
{
	ProcessList processList;
	processList.Update();

	HANDLE ha = GetProcessByName("Skype.exe");

	cout << ha << endl;
	/*for (int i = 0; i < processList.getCount(); i++)
	{
		int pid = processList.getPID(i);
		Process process = Process(pid);
		
	}*/

	system("pause");
}


void ProcessList::Update()
{
	while (true)
	{
		DWORD cb = sizeof(DWORD) * processListCount;
		DWORD cbNeeded = NULL;

		EnumProcesses(this->processList, cb, &cbNeeded);

		if (cb == cbNeeded)
		{
			if (this->processList)
			{
				delete[] this->processList;
				this->processList = nullptr;
			}

			this->processListCount += 128;
			this->processList = new DWORD[this->processListCount];
		}
		else
		{
			this->processListSize = cbNeeded;
			break;
		}			
	}
}


int ProcessList::getCount()
{
	if (processListSize != NULL)
		return  processListSize / sizeof(DWORD);
	else
		return 0;
}

int ProcessList::getPID(const int i)
{
	if (processList[i] != NULL && processList[i] != 0)
		return processList[i];
	else
		return 0;
}

Process* ProcessList::getProcess(const DWORD processId)
{
	// auto variable
	Process *process = new Process(processId);

	return process;
}
