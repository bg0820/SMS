#include "NetworkManager.hpp"

int NetworkManager::Init()
{
	if (!networkListener->Init(socket))
		return 0;

	networkListener->initSocketAddr(8080, "127.0.0.1");

	return 1;
}

int NetworkManager::Start()
{
	if (!networkListener->Connect(socket))
		return 0;

	while (true)
	{
		std::cout << "SEND()" << std::endl;
		char * buf = "SADASDDAS";

		if (!NetworkListener::Send(socket, buf,10))
		{
			return 0;
		}
		//cout << nResult << endl;

		Sleep(100);
	}

	return 1;
}

int NetworkManager::Stop()
{
	if (!networkListener->Disconnect(socket))
		return 0;

	return 1;
}