#include "NetworkManager.hpp"

int NetworkManager::Start()
{
	thread networkThread([&]() {networkProc(socket, addr); });
	networkThread.join();


	return 1;
}

int NetworkManager::Init()
{
	if (!networkListener->Init(socket, addr))
		return 0;

	return 1;
}

int NetworkManager::Stop()
{
	if (!networkListener->Disconnect(socket))
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

		printf("[TCP/%s:%d] %s\n", inet_ntoa(paramAddr.sin_addr), ntohs(paramAddr.sin_port), buf);
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

int main()
{
	cout << "Server Start" << endl;
	cout << "Network Listener..." << endl;
	NetworkManager networkManager("127.0.0.1", 8080);

	if (networkManager.Init())
	{

		cout << "Network Thread Create..." << endl;
		networkManager.Start();
	}

	system("pause");
}