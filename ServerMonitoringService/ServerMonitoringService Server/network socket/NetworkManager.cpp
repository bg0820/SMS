#include "NetworkManager.hpp"

void NetworkManager::Start()
{
	thread networkThread(&networkProc);
	networkThread.join();
}

void NetworkManager::networkProc()
{

	while (true)
	{

	}

}
