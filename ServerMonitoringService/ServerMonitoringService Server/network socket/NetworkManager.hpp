#ifndef _NETWORKMANAGER_H_
#define _NETWORKMANAGER_H_

#include <iostream>
#include "NetworkListener.hpp"
#include <thread>
#ifndef _WINSOCK2API_
#include <WinSock2.h>
#include <Windows.h>
#endif
#pragma comment(lib, "ws2_32.lib") // WinSock2 Lib

using namespace std;

#define PORT 8080
#define LOOPBACK TEXT("127.0.0.1")

class NetworkManager
{
private:
	NetworkListener* networkListener = NULL;
	SOCKET socket;
public:
	NetworkManager()
	{
		networkListener = new NetworkListener;
	}
	~NetworkManager()
	{
		if (networkListener)
		{
			delete networkListener;
			networkListener = nullptr;
		}
	}
	int Init();
	int Start();
	int Stop();
	void networkProc();
	void clientProc(SOCKET client);
};

#endif