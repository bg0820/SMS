#include "NetworkManager.hpp"

int NetworkManager::Init()
{
	if (!networkListener->Init(socket))
		return 0;

	networkListener->initSocketAddr(PORT, "219.250.237.78");

	return 1;
}

int NetworkManager::Start()
{
	if (!networkListener->Connect(socket))
		return 0;

	while (true)
	{
		int len = sizeof(buf);
		if (!NetworkListener::Send(socket, buf, len))
		{
			return 0;
		}
		//cout << nResult << endl;

		Sleep(1000);
	}

	return 1;
}

int NetworkManager::Stop()
{
	if (!networkListener->Disconnect(socket))
		return 0;

	return 1;
}