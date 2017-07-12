#include "ProcessList.hpp"

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

Process ProcessList::getProcess(const DWORD processId)
{
	// auto variable
	Process process = Process(processId);

	return process;
}
