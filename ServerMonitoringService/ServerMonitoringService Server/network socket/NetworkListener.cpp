#include "NetworkListener.hpp"

int NetworkListener::Init(SOCKET &paramSocket, SOCKADDR_IN &parmAddr)
{
	int nResult = 1;
	cout << "Network Listener Initializing..." << endl;

	//WinSock Init
	if (WSAStartup(DllVersion, &wsa) != 0)
		nResult = 0;

	// Socket Create
	paramSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (paramSocket == INVALID_SOCKET) // FAILED
		nResult = 0;

	cout << "Network Listener SocketAddr" << " [" << this->ip << ":" << this->port << "]" << " Initializing..." << endl;
	initSocketAddr(parmAddr, this->port, this->ip, FALSE);

	cout << "Network Listener Bind and Listen..." << endl;
	if (bind(paramSocket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
		nResult = 0;

	if (listen(paramSocket, BACKLOG_COUNT) == SOCKET_ERROR)
		nResult = 0;

	return nResult;
}

void NetworkListener::initSocketAddr(SOCKADDR_IN &paramAddr, u_short port, const TCHAR *ip, BOOLEAN IPv6)
{
	// Socket Init
	if (IPv6)
		addr.sin_family = AF_INET6; // IPv4
	else
		addr.sin_family = AF_INET; // IPv6
	addr.sin_port = htons(port); // Port
	addr.sin_addr.S_un.S_addr = inet_addr(ip); // Broadcast

	paramAddr = addr;
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
