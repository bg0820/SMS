#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <Windows.h>

#define BUFFER_SIZE 1024

class Client
{
public:
	SOCKET socket;
	SOCKADDR_IN addr;
	CHAR recvBuffer[BUFFER_SIZE + 1];
	CHAR sendBuffer[BUFFER_SIZE + 1];
	int recvBytes;
	int sendBytes;
public:
	Client() // SOCKET socket, SOCKADDR_IN addr)
	{
		//this->socket = socket;
		//this->addr = addr;
	}

	~Client()
	{

	}

	SOCKET getSocket();
	SOCKADDR_IN getAddr();
};

#endif