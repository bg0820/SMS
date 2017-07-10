#include "ClientManager.hpp"


BOOLEAN ClientManager::add(const SOCKADDR_IN paramAddr, SOCKET paramClientSocket)
{
	string ip = Util::string_format("%s:%d", inet_ntoa(paramAddr.sin_addr), ntohs(paramAddr.sin_port));

	if (!isContainsKey(paramAddr)) // NOT FOUND
	{
		Client client;
		client.socket = paramClientSocket;
		client.addr = paramAddr;
		client.recvBytes = 0;
		client.sendBytes = 0;

		pair<SOCKADDR_IN, Client> hash = hash_map<SOCKADDR_IN, Client>::value_type(paramAddr, client);
		clientList.insert(hash);

		string str = Util::string_format("Client Connected [%s], Count : %d", ip.c_str(), getCount());
		Log::printLog(str);

		return TRUE;
	}

	return FALSE;
}

void ClientManager::remove(const SOCKADDR_IN paramAddr)
{
	string ip = Util::string_format("%s:%d", inet_ntoa(paramAddr.sin_addr), ntohs(paramAddr.sin_port));

	if (isContainsKey(paramAddr)) // FOUND
	{
		Client client = getValue(paramAddr);
		cout << inet_ntoa(client.addr.sin_addr) << endl;
		
		// 클라이언트 정보 얻기
		SOCKADDR_IN socketAddr;
		int nAddrLength = sizeof(socketAddr);
		getpeername(client.socket, (SOCKADDR*)&socketAddr, &nAddrLength);

		//closesocket(client.socket);

		clientList.erase(paramAddr); // remove

		string str = Util::string_format("Client DisConnected [%s], Count : %d", ip.c_str(), getCount());
		Log::printLog(str);
	}
}


BOOLEAN ClientManager::isContainsKey(const SOCKADDR_IN paramAddr)
{
	string ip = Util::string_format("%s:%d", inet_ntoa(paramAddr.sin_addr), ntohs(paramAddr.sin_port));

	if (clientList.find(paramAddr) == clientList.end())
		return FALSE;
	else
		return TRUE;
}

Client ClientManager::getValue(const SOCKADDR_IN paramAddr)
{
	string ip = Util::string_format("%s:%d", inet_ntoa(paramAddr.sin_addr), ntohs(paramAddr.sin_port));

	hash_map<SOCKADDR_IN, Client>::iterator clientFind = clientList.find(paramAddr);

	// if (client is Exist)
	Client client = clientFind->second;
	return client;

}

int ClientManager::getCount()
{
	return clientList.size();
}
