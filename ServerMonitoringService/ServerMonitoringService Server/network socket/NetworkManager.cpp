#include "NetworkManager.hpp"

int NetworkManager::Start()
{
	thread networkThread(&networkProc);
	networkThread.join();

	return 1;
}

void NetworkManager::networkProc()
{
	while (true)
	{
		if (networkListener->Connect(socket))
		{

		}

	}
}


int NetworkManager::Init()
{
	if (!networkListener->Init(socket))
		return 0;

	networkListener->initSocketAddr(8080, "127.0.0.1");

	return 1;
}



int NetworkManager::Stop()
{
	if (!networkListener->Disconnect(socket))
		return 0;

	return 1;
}