#ifndef _NETWORKMANAGER_H_
#define _NETWORKMANAGER_H_

#include <iostream>
#include "NetworkListener.h"

using namespace std;

#define PORT 8080
#define IP "127.0.0.1"

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
		delete networkListener;
		networkListener = NULL;
	}
public:
	int Init();
	int Start();
	int Stop();
};

#endif