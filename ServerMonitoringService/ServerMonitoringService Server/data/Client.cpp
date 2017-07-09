#include "Client.hpp"

int Client::clientCount = 0;

SOCKET Client::getSocket()
{
	return socket;
}

SOCKADDR_IN Client::getAddr()
{
	return addr;
}