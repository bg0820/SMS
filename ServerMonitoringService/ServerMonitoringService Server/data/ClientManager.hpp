#ifndef _CLIENTMANAGER_H_
#define _CLIENTMANAGER_H_

#include "Client.hpp"

#include "../util/Log.hpp"
#include <Windows.h>
#include <unordered_map>

using namespace std;

class ClientManager
{
private:
	unordered_map<string, Client*> clientList;
	int clientCount = 0;
public:
	Client* add(SOCKADDR_IN paramAddr, SOCKET paramClientSocket);
	Client* remove(SOCKADDR_IN paramAddr);
	BOOLEAN isContainsKey(SOCKADDR_IN paramAddr);
	Client* getValue(SOCKADDR_IN paramAddr);
	//BOOLEAN isContainsValue(Client client);
	int getCount();
};

#endif