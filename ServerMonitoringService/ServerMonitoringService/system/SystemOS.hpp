#ifndef _SYSTEMOS_H_
#define _SYSTEMOS_H_

#include <Windows.h>

#define BUFFER_SIZE 32767

typedef unsigned long       DWORD;

class SystemOS
{
private:
	TCHAR* userName;
	TCHAR* computerName;
	TCHAR* osVersionName;

	int initUserName(TCHAR *&val);
	int initComputerName(TCHAR *&val);
	int initOSVersionName(TCHAR *&val);
public:
	SystemOS()
	{
		initUserName(userName);
		initComputerName(computerName);
		initOSVersionName(osVersionName);
	}

	~SystemOS()
	{
		if (userName)
		{
			delete[] userName;
			userName = nullptr;
		}
		if (computerName)
		{
			delete[] computerName;
			computerName = nullptr;
		}
		if (osVersionName)
		{
			delete[] osVersionName;
			osVersionName = nullptr;
		}
	}

	TCHAR* getUserName();
	TCHAR* getComputerName();
	TCHAR* getOSVersionName();
};

#endif // !_SYSTEMOS_H_
