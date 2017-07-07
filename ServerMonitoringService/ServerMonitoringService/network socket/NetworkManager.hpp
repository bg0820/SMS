#ifndef _NETWORKMANAGER_H_
#define _NETWORKMANAGER_H_

//#include <iostream>
#include "NetworkListener.hpp"

//using namespace std;

#define PORT 8080
#define LOOPBACK "127.0.0.1"

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
public:
	int Init();
	int Start();
	int Stop();
};

#endif