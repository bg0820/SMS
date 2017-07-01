#ifndef _PROCESSMANAGER_H_
#define _PROCESSMANAGER_H_

#include "Process.h"

#define TRUE 1
#define FALSE 0

class ProcessManager
{
private:
	Process * process;
	int numCount;
	int curPosition;
	int maxLen;
public:
	ProcessManager(int len)
	{
		maxLen = len;
		numCount = 0;
		curPosition = -1;
		process = new Process[len];
	}

	~ProcessManager() {
		delete process;
	}

	int insert(Process process); // Insert
	int first(Process &data); // First Move
	int next(Process &data); // Next Move
	Process remove(ProcessManager &processManager); // Current Position Object Remove
	int getCount();
};

#endif
