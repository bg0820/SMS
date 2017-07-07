#include "NetworkManager.hpp"

int NetworkManager::Start()
{
	/*thread networkThread(&networkProc);
	networkThread.join();*/

	return 1;
}
/*
void NetworkManager::clientProc(SOCKET client)
{
	cout << "Á¢¼ÓµÈ Client : " << inet_ntoa(addr.sin_addr) << endl;
}

void NetworkManager::networkProc()
{
	while (true)
	{
		SOCKET client = accept(socket, (SOCKADDR*)&addr, sizeof(addr));

		thread clientThread(&clientProc, client);
		clientThread.join();
	}
}
*/

int NetworkManager::Init()
{
	/*if (!networkListener->Init(socket))
		return 0;

	networkListener->initSocketAddr(8080, "127.0.0.1");

	networkListener->Bind(socket);
	networkListener->Listen(socket);*/

	return 1;
}



int NetworkManager::Stop()
{
	if (!networkListener->Disconnect(socket))
		return 0;

	return 1;
}