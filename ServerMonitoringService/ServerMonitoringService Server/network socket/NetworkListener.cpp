#include "NetworkListener.hpp"


BOOL AddSocketInfo(Client *clientArray, SOCKET clientSocket)
{
	if (Client::clientCount >= (1024 - 1))
	{
		printf("[오류] 소켓 정보를 추가할 수 없습니다.\n");
		return FALSE;
	}

	Client client;

	client.socket = clientSocket;
	client.recvBytes = 0;
	client.sendBytes = 0;

	clientArray[Client::clientCount++] = client;

	return TRUE;
}

void RemoveSocketInfo(Client *clientArray, int nIndex)
{
	Client client = clientArray[nIndex];

	// 클라이언트 정보 얻기
	SOCKADDR_IN socketAddr;
	int nAddrLength = sizeof(socketAddr);
	getpeername(client.socket, (SOCKADDR*)&socketAddr, &nAddrLength);
	printf("[TCP 서버] 클라이언트 종료: IP 주소 = %s, 포트번호 = %d\n",
		inet_ntoa(socketAddr.sin_addr), ntohs(socketAddr.sin_port));

	closesocket(client.socket);

	for (int i = nIndex; i < Client::clientCount; ++i)
		clientArray[i] = clientArray[i + 1];

	Client::clientCount--;
}


int main()
{
	NetworkListener *networkListener = new NetworkListener("127.0.0.1", 8080);
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

	Client *clientArray = new Client[1024];

	while (1)
	{
		// 소켓 셋 초기화
		FD_ZERO(&rset);
		FD_ZERO(&wset);

		// 소켓 셋 지정
		FD_SET(serverSocket, &rset);

		for (int i = 0; i < Client::clientCount; ++i)
		{
			if (clientArray[i].recvBytes > clientArray[i].sendBytes)
				FD_SET(clientArray[i].socket, &wset);
			else
				FD_SET(clientArray[i].socket, &rset);
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
					cout << "ERROR : accept()" << endl;
			}
			else
			{
				printf("[TCP 서버] 클라이언트 접속: IP 주소 = %s, 포트번호 = %d 현재 : %d 명\n",
					inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), Client::clientCount + 1);

				if (AddSocketInfo(clientArray, clientSocket) == FALSE)
				{
					printf("[TCP 서버] 클라이언트 접속을 해제 합니다n");
					closesocket(clientSocket);
				}
			}
		}

		// 소켓 셋 검사 #2 : 데이터 통신
		for (int i = 0; i < Client::clientCount; ++i)
		{
			Client client = clientArray[i];
			if (FD_ISSET(client.socket, &rset))
			{
				// 데이터받기
				retValue = recv(client.socket, client.recvBuffer, BUFFER_SIZE, 0);
				if (retValue == SOCKET_ERROR || retValue == 0)
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{
						cout << "ERROR : recv()" << endl;
						RemoveSocketInfo(clientArray, i);
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
						cout << "ERROR : send()" << endl;
						RemoveSocketInfo(clientArray, i);
						continue;
					}
				}

				client.sendBytes += retValue;

				if (client.recvBytes == client.sendBytes)
					client.recvBytes = client.sendBytes = 0;
			}
		}

	}


	/*
	cout << "Server Start" << endl;
	NetworkManager networkManager("127.0.0.1", 8080);

	err_quit("제목", "내용");
	if (networkManager.Init())
	{
	networkManager.Start();
	}
	*/
	system("pause");
}




int NetworkListener::Init(SOCKET &paramSocket, SOCKADDR_IN &parmAddr, int &paramRetValue)
{
	int nResult = 1;
	int retValue;
	cout << "Network Listener Socket Initializing..." << endl;

	//WinSock Init
	if (WSAStartup(DllVersion, &wsa) != 0)
		nResult = 0;

	// Socket Create
	paramSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (paramSocket == INVALID_SOCKET) // FAILED
		nResult = 0;

	cout << "Network Listener Socket Address" << " [" << this->ip << ":" << this->port << "]" << " Initializing..." << endl;
	initSocketAddr(parmAddr, this->port, this->ip, FALSE);

	cout << "Network Listener Bind and Listen..." << endl;
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
