#include "ClientManager.hpp"


Client* ClientManager::add(SOCKADDR_IN paramAddr, SOCKET paramClientSocket)
{
	string ip = Util::format("%s:%d", inet_ntoa(paramAddr.sin_addr), ntohs(paramAddr.sin_port));

	Client *client = NULL;

	if (!isContainsKey(paramAddr)) // NOT FOUND
	{
		client = new Client;
		client->socket = paramClientSocket;
		client->addr = paramAddr;
		client->recvBytes = 0;
		client->sendBytes = 0;

		clientList.insert(unordered_map<string, Client*>::value_type(ip, client));

		string str = Util::format("Client Connected [%s], Count : %d", ip.c_str(), getCount());
		Log::printLog(str);

		return client;
	}

	return client;
}

Client* ClientManager::remove(SOCKADDR_IN paramAddr)
{
	string ip = Util::format("%s:%d", inet_ntoa(paramAddr.sin_addr), ntohs(paramAddr.sin_port));
	Client *client = NULL;

	if (isContainsKey(paramAddr)) // FOUND
	{
		client = getValue(paramAddr);

		// get Client Info
		SOCKADDR_IN socketAddr;
		int nAddrLength = sizeof(socketAddr);
		getpeername(client->socket, (SOCKADDR*)&socketAddr, &nAddrLength);

		//closesocket(client.socket);

		clientList.erase(ip); // remove

		string str = Util::format("Client DisConnected [%s], Count : %d", ip.c_str(), getCount());
		Log::printLog(str);

		return client;
	}

	return client;
}


BOOLEAN ClientManager::isContainsKey(SOCKADDR_IN paramAddr)
{
	string ip = Util::format("%s:%d", inet_ntoa(paramAddr.sin_addr), ntohs(paramAddr.sin_port));

	if (clientList.find(ip) == clientList.end())
		return FALSE;
	else
		return TRUE;
}

Client* ClientManager::getValue(SOCKADDR_IN paramAddr)
{
	string ip = Util::format("%s:%d", inet_ntoa(paramAddr.sin_addr), ntohs(paramAddr.sin_port));

	Client* client = NULL;

	if (isContainsKey(paramAddr))
	{
		client = clientList.find(ip)->second;
	}
	
	return client;
}

int ClientManager::getCount()
{
	return clientList.size();
}
