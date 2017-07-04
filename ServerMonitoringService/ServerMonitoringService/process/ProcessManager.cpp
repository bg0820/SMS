#include "ProcessManager.hpp"

int ProcessManager::insert(Process process)
{
	if (numCount >= maxLen) // Storage Lack
		return 0;

	ProcessManager::process[numCount] = process;
	numCount++;

	return 1;
}

int ProcessManager::first(Process &data)
{
	if (numCount == 0)
		return FALSE;

	curPosition = 0;
	data = ProcessManager::process[0];

	return TRUE;
}

int ProcessManager::next(Process &data)
{
	if (curPosition >= numCount - 1) // next data dosen't Exist
		return FALSE;

	curPosition++;
	data = ProcessManager::process[curPosition];

	return TRUE;
}

Process ProcessManager::remove(ProcessManager &processManager)
{
	int rpos = curPosition;
	int num = numCount;

	Process process = ProcessManager::process[rpos];

	for (int i = 0; i < num - 1; i++)
		ProcessManager::process[i] = ProcessManager::process[i + 1]; // << 1

																	 // << 1
	numCount--;
	curPosition--;

	return process;
}

int ProcessManager::getCount()
{
	return numCount;
}