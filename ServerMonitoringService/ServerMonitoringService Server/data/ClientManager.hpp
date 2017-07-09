#ifndef _CLIENTMANAGER_H_
#define _CLIENTMANAGER_H_

#include "Client.hpp"
#include <Windows.h>

#define MAX_USER 1024

class ClientManager
{
public:
	Client *client;
	int clientCount;
public:
	ClientManager() // SOCKET socket, SOCKADDR_IN addr)
	{
		//this->socket = socket;
		//this->addr = addr;
	}

	~ClientManager()
	{

	}


};

#endif