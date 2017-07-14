#ifndef _MAIN_H_
#define _MAIN_H_

#include <iostream>
#include <windows.h>	
#include <Shlobj.h>
#include "data/DataManager.hpp"
#include "util/CSerivce.hpp"

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

using namespace std;

#define KB				1024						// KB
#define MB				1024 / 1024					// MB
#define GB				1024 / 1024 / 1024			// GB


// Internal name of the service 
#define SERVICE_NAME             TEXT("Service Monitoring Service Agent")

// Displayed name of the service 
#define SERVICE_DISPLAY_NAME     TEXT("Service Monitoring Service")

// Service start options. 
#define SERVICE_START_TYPE       SERVICE_DEMAND_START 

// List of service dependencies - "dep1\0dep2\0\0" 
#define SERVICE_DEPENDENCIES     TEXT("")

// The name of the account under which the service should run 
#define SERVICE_ACCOUNT          TEXT("NT AUTHORITY\\LocalService")

// The password to the service account name 
#define SERVICE_PASSWORD         NULL


#endif