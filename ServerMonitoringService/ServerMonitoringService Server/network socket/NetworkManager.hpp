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
	SOCKET serverSocket;
	SOCKADDR_IN addr;
	TCHAR *ip;
	USHORT port;
public:
	NetworkManager(TCHAR *ip, USHORT port) : ip(ip), port(port)
	{
		cout << "Network Listener Creating..." << endl;
		networkListener = new NetworkListener(ip, port);
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
	void clientProc(SOCKET client, SOCKADDR_IN paramAddr);
	void networkProc(SOCKET serverSocket, SOCKADDR_IN paramAddr);
};

#endif