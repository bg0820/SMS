#include "Client.hpp"

SOCKET Client::getSocket()
{
	return socket;
}

SOCKADDR_IN Client::getAddr()
{
	return addr;
}