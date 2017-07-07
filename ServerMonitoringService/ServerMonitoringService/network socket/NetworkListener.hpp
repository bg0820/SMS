#ifndef _NETWORKLISTENER_H_
#define _NETWORKLISTENER_H_

#include <Windows.h>

#pragma comment(lib, "ws2_32.lib") // WinSock2 Lib

//#include <iostream>
//using namespace std;

#define RECEIVE_BUFFER_SIZE 1024
#define SEND_BUFFER_SIZE 1024

class NetworkListener
{
private:
	WSADATA wsa;
	SOCKADDR_IN addr;
	WORD DllVersion = MAKEWORD(2, 2);
public:
	NetworkListener()
	{

	}

	~NetworkListener()
	{
		WSACleanup();
	}

	int Init(SOCKET &parmSocket);
	int Connect(SOCKET socket);
	void initSocketAddr(u_short port, const char * ip);
	static int Send(SOCKET socket, char * buf, int bufSize);
	static int Recv(SOCKET socket, char * buf, int bufSize);
	int Disconnect(SOCKET socket);
};

#endif