#ifndef _NETWORKMANAGER_H_
#define _NETWORKMANAGER_H_

#include "NetworkListener.h"

class NetworkManager
{
private:
public:
	void Start();
	void Stop();
	void networkProc();
};

#endif