#include "ClientManager.hpp"


BOOLEAN ClientManager::add(SOCKADDR_IN paramAddr, SOCKET paramClientSocket)
{
	if (clientList.find(inet_ntoa(paramAddr.sin_addr)) == clientList.end())
	{
		cout << "FIND" << endl;

		Client client;
		client.socket = paramClientSocket;
		client.recvBytes = 0;
		client.sendBytes = 0;
		client.ip = inet_ntoa(paramAddr.sin_addr);

		std::pair<string, Client> hash = hash_map<string, Client>::value_type(client.ip, client);
		clientList.insert(hash);

		return TRUE;
	}

	return FALSE;
}

void ClientManager::remove(SOCKADDR_IN paramAddr)
{
	TCHAR *buffer = "[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ� = %s, ��Ʈ��ȣ = %d\n";
	sprintf(buffer, inet_ntoa(paramAddr.sin_addr), ntohs(paramAddr.sin_port));
	Log::printLog(buffer);
	
	hash_map<string, Client>::iterator Iter = clientList.lower_bound(inet_ntoa(paramAddr.sin_addr));
	Client client = Iter->second;
	closesocket(client.socket);

	// Ŭ���̾�Ʈ ���� ���
	SOCKADDR_IN socketAddr;
	int nAddrLength = sizeof(socketAddr);
	getpeername(client.socket, (SOCKADDR*)&socketAddr, &nAddrLength);
}

BOOLEAN ClientManager::isContainsKey(string ip)
{
	return BOOLEAN();
}

Client ClientManager::getValue(SOCKADDR_IN paramAddr)
{
	hash_map<string, Client>::iterator Iter = clientList.lower_bound(inet_ntoa(paramAddr.sin_addr));
	Client client = Iter->second;
	return client;
}

int ClientManager::getCount()
{
	return clientList.size();
}
