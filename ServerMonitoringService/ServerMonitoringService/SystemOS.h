#ifndef _SYSTEMOS_H_
#define _SYSTEMOS_H_

#include <Windows.h>

#define BUFFER_SIZE 32767

class SystemOS
{
private:
	char * userName;
	char * computerName;
	char * osName;
public:
	int getUserName(char *& val);
	int getComputerName(char *& val);
};

#endif // !_SYSTEMOS_H_
