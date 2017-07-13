#ifndef _DATAMANAGER_H_
#define _DATAMANAGER_H_

#include <Windows.h>

class DataManager
{
private:
	DWORD processList[1024];
public:
	void Update();
};

#endif