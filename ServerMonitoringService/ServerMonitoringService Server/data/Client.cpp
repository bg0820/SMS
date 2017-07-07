#include "Client.h"

SOCKET Client::getSocket()
{
	return socket;
}

SOCKADDR_IN Client::getAddr()
{
	return addr;
}