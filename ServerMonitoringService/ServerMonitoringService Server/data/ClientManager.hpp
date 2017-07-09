#ifndef _CLIENTMANAGER_H_
#define _CLIENTMANAGER_H_

#include "Client.hpp"

#include "../util/Log.hpp"
#include <Windows.h>
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS 1
#include <hash_map>

#define MAX_USER 1024

using namespace std;

class ClientManager
{
private:
	hash_map<string, Client> clientList;
	int clientCount = 0;
public:
	BOOLEAN add(SOCKADDR_IN paramAddr, SOCKET paramClientSocket);
	void remove(SOCKADDR_IN paramAddr);
	BOOLEAN isContainsKey(string ip);
	Client getValue(SOCKADDR_IN paramAddr);
	//BOOLEAN isContainsValue(Client client);
	int getCount();
};

#endif