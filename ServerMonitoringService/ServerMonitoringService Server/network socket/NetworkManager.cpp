#include "NetworkManager.hpp"

int NetworkManager::Start()
{
	cout << "Network Thread Create..." << endl;
	thread networkThread([&]() {networkProc(serverSocket, addr); });
	networkThread.join();

	return 1;
}

int NetworkManager::Init()
{
	/*if (!networkListener->Init(this->serverSocket, this->addr))
		return 0;*/

	return 1;
}

int NetworkManager::Stop()
{
	if (!networkListener->Disconnect(this->serverSocket))
		return 0;

	return 1;
}

void NetworkManager::clientProc(SOCKET client, SOCKADDR_IN paramAddr)
{
	cout << "Client Conneced IP : " << inet_ntoa(paramAddr.sin_addr) << endl;

	char buf[BUFFER_SIZE + 1];

	while (true)
	{
		int len = recv(client, buf, BUFFER_SIZE, 0);
		if (len == SOCKET_ERROR || len == 0) {
			break;
		}

		buf[BUFFER_SIZE] = '\0';

		//printf("[TCP/%s:%d] %s\n", inet_ntoa(paramAddr.sin_addr), ntohs(paramAddr.sin_port), buf);
	}

	//select()

	cout << "DisConnected IP : " << inet_ntoa(paramAddr.sin_addr) << endl;
}

void NetworkManager::networkProc(SOCKET serverSocket, SOCKADDR_IN paramAddr)
{
	while (true)
	{
		cout << "Client Connect Wating..." << endl;
		int size = sizeof(paramAddr);
		SOCKET client = accept(serverSocket, (SOCKADDR*)&paramAddr, &size);

		thread clientThread([&]() {clientProc(client, paramAddr); });
		clientThread.join();
	}
}
