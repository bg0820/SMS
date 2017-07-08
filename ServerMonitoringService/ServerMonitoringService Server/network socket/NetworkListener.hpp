#ifndef _NETWORKLISTENER_H_
#define _NETWORKLISTENER_H_

#include <iostream>
#ifndef _WINSOCK2API_
#include <WinSock2.h>
#include <Windows.h>
#endif

#pragma comment(lib, "ws2_32.lib") // WinSock2 Lib

using std::cout;
using std::endl;

#define RECEIVE_BUFFER_SIZE 1024
#define SEND_BUFFER_SIZE 1024
#define BUFFER_SIZE 1024
#define BACKLOG_COUNT 128

class NetworkListener
{
private:
	WSADATA wsa;
	SOCKADDR_IN addr;
	WORD DllVersion = MAKEWORD(2, 2);
	TCHAR *ip;
	USHORT port;
private:
	void initSocketAddr(SOCKADDR_IN &paramAddr, u_short port, const TCHAR *ip, BOOLEAN IPv6);
public:
	NetworkListener(TCHAR *ip, USHORT port) : ip(ip), port(port)
	{

	}

	~NetworkListener()
	{
		WSACleanup();
	}

	int Init(SOCKET &pSocket, SOCKADDR_IN &parmAddr);
	static int Send(SOCKET socket, TCHAR *buf, int bufSize);
	static int Recv(SOCKET socket, TCHAR *buf, int bufSize);
	int Disconnect(SOCKET socket);
};

#endif