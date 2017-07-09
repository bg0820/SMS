#include "NetworkListener.hpp"
#include "../data/ClientManager.hpp"
#include <atlstr.h>

int main()
{
	string localIp = Util::getLocalIp();

	string str = Util::string_format("Local IP : %s", localIp.c_str());
	Log::printLog(str);

	TCHAR ip[16];
	_tcscpy_s(ip, CA2T(localIp.c_str())); // string to TCHAR

	NetworkListener *networkListener = new NetworkListener(ip, 41850);
	SOCKET serverSocket;
	SOCKADDR_IN addr;

	int retValue;
	if (!networkListener->Init(serverSocket, addr, retValue))
		return 0;

	FD_SET rset;
	FD_SET wset;
	SOCKET clientSocket;
	SOCKADDR_IN clientAddr;
	int nAddrLength;

	ClientManager clientManager;

	while (TRUE)
	{
		// 소켓 셋 초기화
		FD_ZERO(&rset);
		FD_ZERO(&wset);

		// 소켓 셋 지정
		FD_SET(serverSocket, &rset);

		for (int i = 0; i < clientManager.getCount(); ++i)
		{
			if (clientManager.getValue(clientAddr).recvBytes > clientManager.getValue(clientAddr).sendBytes)
				FD_SET(clientManager.getValue(clientAddr).socket, &wset);
			else
				FD_SET(clientManager.getValue(clientAddr).socket, &rset);
		}

		// select()
		if ((retValue = select(0, &rset, &wset, NULL, NULL)) == SOCKET_ERROR)
		{
			return 0;
		}

		// 소켓 셋 검사 #1 : 클라이언트 접속
		if (FD_ISSET(serverSocket, &rset))
		{
			nAddrLength = sizeof(clientAddr);
			if ((clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &nAddrLength)) == INVALID_SOCKET)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
					Log::printLog("Error : accept()");
			}
			else
			{
				if (clientManager.add(clientAddr, clientSocket) == FALSE)
				{
					Log::printLog("Client Disconnected");
					closesocket(clientSocket);
				}
			}
		}

		// 소켓 셋 검사 #2 : 데이터 통신
		for (int i = 0; i < clientManager.getCount(); ++i)
		{
			Client client = clientManager.getValue(clientAddr);
			if (FD_ISSET(client.socket, &rset))
			{
				// 데이터받기
				retValue = recv(client.socket, client.recvBuffer, BUFFER_SIZE, 0);
				if (retValue == SOCKET_ERROR || retValue == 0)
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{
						Log::printLog("Error : recv()");
						clientManager.remove(clientAddr);
						continue;
					}
				}
				else
				{
					client.recvBytes = retValue;

					// 받은 데이터 출력
					SOCKADDR_IN sockAddr;
					int nAddrLength = sizeof(sockAddr);
					getpeername(client.socket, (SOCKADDR*)&sockAddr, &nAddrLength);

					client.recvBuffer[retValue] = '\0';
					//printf("[TCP/%s:%d] %s\n", inet_ntoa(sockAddr.sin_addr), ntohs(sockAddr.sin_port), client.recvBuffer);
				}
			}

			if (FD_ISSET(client.socket, &wset))
			{
				// 데이터보내기
				retValue = send(client.socket, client.sendBuffer + client.sendBytes, client.recvBytes - client.sendBytes, 0);

				if (retValue == SOCKET_ERROR)
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{
						Log::printLog("Error : send()");
						clientManager.remove(clientAddr);
						continue;
					}
				}

				client.sendBytes += retValue;

				if (client.recvBytes == client.sendBytes)
					client.recvBytes = client.sendBytes = 0;
			}
		}

	}

	system("pause");
}




int NetworkListener::Init(SOCKET &paramSocket, SOCKADDR_IN &parmAddr, int &paramRetValue)
{
	int nResult = 1;
	int retValue;
	Log::printLog("Network Listener Socket Initializing...");

	//WinSock Init
	if (WSAStartup(DllVersion, &wsa) != 0)
		nResult = 0;

	// Socket Create
	paramSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (paramSocket == INVALID_SOCKET) // FAILED
		nResult = 0;

	string str = Util::string_format("Network Listener Socket Address [%s:%d] Initializing...", this->ip, this->port);
	Log::printLog(str);

	initSocketAddr(parmAddr, this->port, this->ip, FALSE);

	Log::printLog("Network Listener Bind and Listen...");
	if ((retValue = bind(paramSocket, (SOCKADDR*)&addr, sizeof(addr))) == SOCKET_ERROR)
		nResult = 0;

	if ((retValue = listen(paramSocket, BACKLOG_COUNT)) == SOCKET_ERROR)
		nResult = 0;

	// non blocking mode
	ULONG mode = TRUE;
	if ((retValue = ioctlsocket(paramSocket, FIONBIO, &mode)) == SOCKET_ERROR)
		nResult = 0;


	/*
	FD_SET readFds;
	FD_SET writeFds;
	FD_ZERO(&readFds);
	FD_ZERO(&writeFds);

	// client array initialize
	memset(client, -1, sizeof(client) / sizeof(int));

	FD_SET(paramSocket, &readFds);

	int state;

	state = select()
	*/

	paramRetValue = retValue;
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

int NetworkListener::Disconnect(SOCKET socket)
{
	if (closesocket(socket) == SOCKET_ERROR) // if function success return value is 0
		return 0;

	return 1;
}
