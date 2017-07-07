#ifndef _NETWORKLISTENER_H_
#define _NETWORKLISTENER_H_

#ifndef _WINSOCK2API_
#include <WinSock2.h>
#include <Windows.h>
#endif

#pragma comment(lib, "ws2_32.lib") // WinSock2 Lib

#define RECEIVE_BUFFER_SIZE 1024
#define SEND_BUFFER_SIZE 1024

class NetworkListener
{
private:
	WSADATA wsa;
	SOCKADDR_IN addr;
	WORD DllVersion = MAKEWORD(2, 1);
public:
	int Init(SOCKET &pSocket);
	int Connect(SOCKET socket);
	void initSocketAddr(u_short port, const TCHAR *ip);
	static int Send(SOCKET socket, TCHAR *buf, int bufSize);
	static int Recv(SOCKET socket, TCHAR *buf, int bufSize);
	int Disconnect(SOCKET socket);
};

#endif