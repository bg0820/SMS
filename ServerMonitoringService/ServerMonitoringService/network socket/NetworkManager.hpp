#ifndef _NETWORKMANAGER_H_
#define _NETWORKMANAGER_H_

#include <iostream>
#include "NetworkListener.hpp"

#include <Windows.h>

#pragma comment(lib, "ws2_32.lib") // WinSock2 Lib
//using namespace std;

#define PORT 41850
#define LOOPBACK TEXT("127.0.0.1")

class NetworkManager
{
private:
	NetworkListener* networkListener = NULL;
	SOCKET socket;
	TCHAR buf[SEND_BUFFER_SIZE];
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
public:
	int Init();
	int Start();
	int Stop();
};

#endif