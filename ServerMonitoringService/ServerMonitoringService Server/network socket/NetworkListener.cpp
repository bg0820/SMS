#include "NetworkListener.hpp"

int NetworkListener::Init(SOCKET &parmSocket)
{
	//WinSock Init
	if (WSAStartup(DllVersion, &wsa) != 0)
		return 0;

	// Socket Create
	parmSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (parmSocket == INVALID_SOCKET) // FAILED
		return 0;

	return 1;
}

int NetworkListener::Bind(SOCKET socket)
{
	if (bind(socket, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
		return 0;

	return 1;
}

int NetworkListener::Listen(SOCKET socket)
{
	if (listen(socket, SOMAXCONN) == SOCKET_ERROR)
		return 0;

	return 1;
}

void NetworkListener::initSocketAddr(u_short port, const TCHAR *ip)
{
	// Socket Init
	addr.sin_family = AF_INET; // IPv4
	addr.sin_port = htons(port); // Port
	addr.sin_addr.S_un.S_addr = inet_addr(ip); // Broadcast
}

int NetworkListener::Send(SOCKET socket, TCHAR *buf, int bufSize)
{
	if (send(socket, buf, sizeof(buf), NULL) == SOCKET_ERROR) // if function success return value is 0
		return 0;

	return 1;
}

int NetworkListener::Recv(SOCKET socket, TCHAR *buf, int bufSize)
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