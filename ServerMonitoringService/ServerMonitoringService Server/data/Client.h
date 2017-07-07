#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <Windows.h>

class Client
{
private:
	SOCKET socket;
	SOCKADDR_IN addr;
public:
	Client(SOCKET socket, SOCKADDR_IN addr)
	{
		this->socket = socket;
		this->addr = addr;
	}

	~Client()
	{

	}

	SOCKET getSocket();
	SOCKADDR_IN getAddr();
};

#endif