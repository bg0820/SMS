#include "ProcessList.hpp"

int ProcessList::update()
{
	if (EnumProcesses(processList, sizeof(processList), &processListSize) == 0) // EnumProcesses func FAILED return value is zero;
		return 0;

	return 1; // SUCCESS
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
