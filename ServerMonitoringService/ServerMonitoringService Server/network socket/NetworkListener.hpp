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
/*
	Thread Class
*/
#include "../util/Thread.hpp"

#pragma comment(lib, "ws2_32.lib") // WinSock2 Lib

#define RECEIVE_BUFFER_SIZE 1024
#define SEND_BUFFER_SIZE 1024
#define BUFFER_SIZE 1024
#define BACKLOG_COUNT 128


typedef struct socketinfo
{
	OVERLAPPED overlapped;
	WSABUF dataBuffer;
	SOCKET socket;
	char messageBuffer[BUFFER_SIZE];
	int receiveBytes;
	int sendBytes;
}SOCKETINFO;

class NetworkListener
{
private:
	int clientCount = 0;

	int threadCount = 0;
	HANDLE *hThread = NULL;

	TCHAR *serverIp;
	USHORT serverPort;
	WORD DllVersion = MAKEWORD(2, 2);
	WSADATA wsa;
	SOCKET serverSocket;
	SOCKADDR_IN serverAddr;
	HANDLE hIOCP;
	int retValue;
private:
	int Init(SOCKET &pSocket, SOCKADDR_IN &paramAddr, int &paramRetValue, HANDLE &handle);
	void initSocketAddr(BOOLEAN IPv6);
	DWORD callbackProc(LPVOID hIOCP);
public:
	NetworkListener(TCHAR *ip, USHORT port) : serverIp(ip), serverPort(port)
	{
		// threadCount = cpu core processcor
		SYSTEM_INFO systemInfo;
		GetSystemInfo(&systemInfo);
		this->threadCount = systemInfo.dwNumberOfProcessors;
		Log::printLog(Util::format("Create Thread Count : %d", this->threadCount));
		this->hThread = new HANDLE[this->threadCount];

		this->Init(this->serverSocket, this->serverAddr, this->retValue, this->hIOCP);

	}

	~NetworkListener()
	{
		// Close Thread Handle
		for (int i = 0; i < this->threadCount; i++)
		{
			CloseHandle(this->hThread[i]);
		}

		WSACleanup();
	}

	void Start();
	void Stop();
	void clientProc(SOCKET client, SOCKADDR_IN paramAddr);
	void networkProc(SOCKET serverSocket, SOCKADDR_IN paramAddr);
	int getRetValue();
	TCHAR* getIp();
	USHORT getPort();
	SOCKET getSocket();
	HANDLE getHIOCP();
	SOCKADDR_IN getSockAddr();
	int Disconnect(SOCKET socket);

};

#endif