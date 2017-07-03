#ifndef _SYSTEMOS_H_
#define _SYSTEMOS_H_

#include <Windows.h>

#define BUFFER_SIZE 32767

typedef unsigned long       DWORD;

class SystemOS
{
private:
	char * userName;
	char * computerName;
	char * osName;

	int initUserName(TCHAR *& val);
	int initComputerName(TCHAR *& val);
public:
	SystemOS()
	{
		initUserName(userName);
		initComputerName(computerName);
	}

	~SystemOS()
	{
		delete userName;
		delete computerName;
	}

	char* getUserName();
	char* getComputerName();
};

#endif // !_SYSTEMOS_H_
