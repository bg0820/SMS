#ifndef _PROCESSLIST_H_
#define _PROCESSLIST_H_

#include <iostream>
#include <Windows.h>
#include <Psapi.h>
#include "Process.hpp"

#pragma comment(lib, "psapi.lib")

using namespace std;

class ProcessList
{
private:
	DWORD processList[1024];
	DWORD processListSize = NULL;

public:
	int update(); // process list update
	int getCount(); // update -> return value is processList count
	Process getProcess(const DWORD processId); // return value is processId index to Process Obj
	int getPID(const int i); // return value is num i to process PID value
							 // TODO: function Add
							 // getProcessUser, getProcessStatus, getProcessNetworkSend, 
							 // getProcessNetworkReceive, getProcessDescription
};

#endif