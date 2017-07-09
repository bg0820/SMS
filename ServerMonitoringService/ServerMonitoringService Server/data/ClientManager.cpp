#include "ClientManager.hpp"


BOOLEAN ClientManager::add(SOCKADDR_IN paramAddr, SOCKET paramClientSocket)
{
	string ip = Util::string_format("%s:%d", inet_ntoa(paramAddr.sin_addr), ntohs(paramAddr.sin_port));

	if (clientList.find(ip) == clientList.end()) // NOT FOUND
	{
		Client client;
		client.socket = paramClientSocket;
		client.addr = paramAddr;
		client.recvBytes = 0;
		client.sendBytes = 0;

		pair<string, Client> hash = hash_map<string, Client>::value_type(ip, client);
		clientList.insert(hash);

		string str = Util::string_format("Client Connected [%s], Count : %d", ip.c_str(), getCount());
		Log::printLog(str);

		return TRUE;
	}

	return FALSE;
}

void ClientManager::remove(SOCKADDR_IN paramAddr)
{
	string ip = Util::string_format("%s:%d", inet_ntoa(paramAddr.sin_addr), ntohs(paramAddr.sin_port));

	if (clientList.find(ip) != clientList.end()) // FOUND
	{
		hash_map<string, Client>::iterator Iter = clientList.lower_bound(ip);
		Client client = Iter->second;
		closesocket(client.socket);

		// 클라이언트 정보 얻기
		SOCKADDR_IN socketAddr;
		int nAddrLength = sizeof(socketAddr);
		getpeername(client.socket, (SOCKADDR*)&socketAddr, &nAddrLength);

		clientList.erase(ip); // remove

		string str = Util::string_format("Client DisConnected [%s], Count : %d", ip.c_str(), getCount());
		Log::printLog(str);
	}
}

BOOLEAN ClientManager::isContainsKey(SOCKADDR_IN paramAddr)
{
	string ip = Util::string_format("%s:%d", inet_ntoa(paramAddr.sin_addr), ntohs(paramAddr.sin_port));

	if (clientList.find(ip) == clientList.end())
		return FALSE;
	else
		return TRUE;
}

Client ClientManager::getValue(SOCKADDR_IN paramAddr)
{
	string ip = Util::string_format("%s:%d", inet_ntoa(paramAddr.sin_addr), ntohs(paramAddr.sin_port));

	hash_map<string, Client>::iterator Iter = clientList.lower_bound(ip);
	Client client = Iter->second;

	return client;
}

int ClientManager::getCount()
{
	return clientList.size();
}
