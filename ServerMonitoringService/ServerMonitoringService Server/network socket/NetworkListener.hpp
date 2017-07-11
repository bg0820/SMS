#ifndef _NETWORKLISTENER_H_
#define _NETWORKLISTENER_H_

#include <iostream>
#ifndef _WINSOCK2API_
#include <WinSock2.h>
#include <Windows.h>
#endif
#include <thread>
#include "../data/Client.hpp"
#include "../data/ClientManager.hpp"
#include <atlstr.h>

#pragma comment(lib, "ws2_32.lib") // WinSock2 Lib

#define RECEIVE_BUFFER_SIZE 1024
#define SEND_BUFFER_SIZE 1024
#define BUFFER_SIZE 1024
#define BACKLOG_COUNT 128

class NetworkListener
{
private:
	TCHAR *serverIp;
	USHORT serverPort;
	WORD DllVersion = MAKEWORD(2, 2);
	WSADATA wsa;
	SOCKET serverSocket;
	SOCKADDR_IN serverAddr;
	int retValue;
private:
	int Init(SOCKET &pSocket, SOCKADDR_IN &paramAddr, int &paramRetValue);
	void initSocketAddr(BOOLEAN IPv6);
public:
	NetworkListener(TCHAR *ip, USHORT port) : serverIp(ip), serverPort(port)
	{
		this->Init(this->serverSocket, this->serverAddr, this->retValue);
	}

	~NetworkListener()
	{
		WSACleanup();
	}

	void clientProc(SOCKET client, SOCKADDR_IN paramAddr);
	void networkProc(SOCKET serverSocket, SOCKADDR_IN paramAddr);
	int getRetValue();
	TCHAR* getIp();
	USHORT getPort();
	SOCKET getSocket();
	SOCKADDR_IN getSockAddr();
	int Disconnect(SOCKET socket);

};

#endif