#include "NetworkListener.hpp"

int NetworkListener::Init(SOCKET &paramSocket)
{
	//WinSock Init
	if (WSAStartup(DllVersion, &wsa) != 0)
		return 0;

	// Socket Create
	paramSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (paramSocket == INVALID_SOCKET) // FAILED
		return 0;

	return 1;
}

void NetworkListener::initSocketAddr(u_short port, const char * ip)
{
	// Socket Init
	addr.sin_family = AF_INET; // IPv4
	addr.sin_port = htons(port); // Port
	addr.sin_addr.S_un.S_addr = inet_addr(ip); // Broadcast
}

int NetworkListener::Connect(SOCKET socket)
{
	if (connect(socket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) // if function success return value is 0
		return 0;
	
	// TODO : if(result == 0) ~~~

	return 1;
}

int NetworkListener::Send(SOCKET socket, char * buf, int bufSize)
{
	if (send(socket, buf, bufSize, NULL) == SOCKET_ERROR) // if function success return value is 0
		return 0;

	return 1;
}

int NetworkListener::Recv(SOCKET socket, char * buf, int bufSize)
{
	if (recv(socket, buf, bufSize, NULL) == SOCKET_ERROR) // if function success return value is 0
		return 0;

	// if(retval == 0) DisConnection

	return 1;
}

int NetworkListener::Disconnect(SOCKET socket)
{
	if (closesocket(socket) == SOCKET_ERROR) // if function success return value is 0
		return 0;

	return 1;
}