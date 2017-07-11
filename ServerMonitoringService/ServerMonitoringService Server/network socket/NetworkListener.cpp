#include "NetworkListener.hpp"



#pragma comment(lib, "Ws2_32.lib")

#define MAX_BUFFER        1024
#define SERVER_PORT        41850

struct SOCKETINFO
{
	WSAOVERLAPPED overlapped;
	WSABUF dataBuffer;
	SOCKET socket;
	char messageBuffer[MAX_BUFFER];
	int receiveBytes;
	int sendBytes;
};

DWORD WINAPI makeThread(LPVOID hIOCP);

int _tmain(int argc, _TCHAR* argv[])
{
	// Winsock Start - windock.dll 로드
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		printf("Error - Can not load 'winsock.dll' file\n");
		return 1;
	}

	// 1. 소켓생성  
	SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (listenSocket == INVALID_SOCKET)
	{
		printf("Error - Invalid socket\n");
		return 1;
	}

	// 서버정보 객체설정
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	// 2. 소켓설정
	if (::bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		printf("Error - Fail bind\n");
		// 6. 소켓종료
		closesocket(listenSocket);
		// Winsock End
		WSACleanup();
		return 1;
	}

	// 3. 수신대기열생성
	if (listen(listenSocket, 5) == SOCKET_ERROR)
	{
		printf("Error - Fail listen\n");
		// 6. 소켓종료
		closesocket(listenSocket);
		// Winsock End
		WSACleanup();
		return 1;
	}

	// 완료결과를 처리하는 객체(CP : Completion Port) 생성
	HANDLE hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	// 워커스레드 생성
	// - CPU * 2개
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	int threadCount = systemInfo.dwNumberOfProcessors * 2;
	unsigned long threadId;
	// - thread Handler 선언
	HANDLE *hThread = new HANDLE[threadCount]; // (HANDLE *)malloc(threadCount * sizeof(HANDLE));
	// - thread 생성
	for (int i = 0; i < threadCount; i++)
	{
		hThread[i] = CreateThread(NULL, 0, makeThread, &hIOCP, 0, &threadId);
	}

	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(SOCKADDR_IN);
	memset(&clientAddr, 0, addrLen);
	SOCKET clientSocket;
	SOCKETINFO *socketInfo;
	DWORD receiveBytes;
	DWORD flags;

	while (1)
	{
		clientSocket = accept(listenSocket, (struct sockaddr *)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET)
		{
			printf("Error - Accept Failure\n");
			return 1;
		}

		// TODO : CLIENT OBJECT CREATE
		socketInfo = new SOCKETINFO; // (struct SOCKETINFO *)malloc(sizeof(struct SOCKETINFO));
		memset((void *)socketInfo, 0x00, sizeof(struct SOCKETINFO));
		socketInfo->socket = clientSocket;
		socketInfo->receiveBytes = 0;
		socketInfo->sendBytes = 0;
		socketInfo->dataBuffer.len = MAX_BUFFER;
		socketInfo->dataBuffer.buf = socketInfo->messageBuffer;
		flags = 0;

		hIOCP = CreateIoCompletionPort((HANDLE)clientSocket, hIOCP, (DWORD)socketInfo, 0);

		// 중첩 소캣을 지정하고 완료시 실행될 함수를 넘겨준다.
		if (WSARecv(socketInfo->socket, &socketInfo->dataBuffer, 1, &receiveBytes, &flags, &(socketInfo->overlapped), NULL))
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				printf("Error - IO pending Failure\n");
				return 1;
			}
		}
	}

	// 6-2. 리슨 소켓종료
	closesocket(listenSocket);

	// Winsock End
	WSACleanup();

	return 0;
}


DWORD WINAPI makeThread(LPVOID hIOCP)
{
	HANDLE threadHandler = *((HANDLE *)hIOCP);
	DWORD receiveBytes;
	DWORD sendBytes;
	ULONG_PTR completionKey;

	struct SOCKETINFO *eventSocket;
	while (1)
	{
		// 입출력 완료 대기
		if (GetQueuedCompletionStatus(threadHandler, &receiveBytes, &completionKey, (LPOVERLAPPED *)&eventSocket, INFINITE) == 0)
		{
			printf("Error - GetQueuedCompletionStatus Failure\n");
			closesocket(eventSocket->socket);
			free(eventSocket);
			return 1;
		}

		eventSocket->dataBuffer.len = receiveBytes;

		if (receiveBytes == 0)
		{
			closesocket(eventSocket->socket);
			free(eventSocket);
			continue;
		}
		else
		{
			printf("TRACE - Receive message : %s (%d bytes)\n", eventSocket->dataBuffer.buf, eventSocket->dataBuffer.len);

			if (WSASend(eventSocket->socket, &(eventSocket->dataBuffer), 1, &sendBytes, 0, NULL, NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
					printf("Error - Fail WSASend(error_code : %d)\n", WSAGetLastError());
				}
			}

			printf("TRACE - Send message : %s (%d bytes)\n", eventSocket->dataBuffer.buf, eventSocket->dataBuffer.len);

			memset(eventSocket->messageBuffer, 0x00, MAX_BUFFER);
			eventSocket->receiveBytes = 0;
			eventSocket->sendBytes = 0;
			eventSocket->dataBuffer.len = MAX_BUFFER;
			eventSocket->dataBuffer.buf = eventSocket->messageBuffer;
			DWORD flags = 0;

			if (WSARecv(eventSocket->socket, &(eventSocket->dataBuffer), 1, &receiveBytes, &flags, &eventSocket->overlapped, NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
					printf("Error - Fail WSARecv(error_code : %d)\n", WSAGetLastError());
				}
			}
		}
	}
}

/*
int main()
{


	string localIp = Util::getLocalIp();

	string str = Util::string_format("Local IP : %s", localIp.c_str());
	Log::printLog(str);

	TCHAR ip[16];
	_tcscpy_s(ip, CA2T(localIp.c_str())); // string to TCHAR

	NetworkListener *networkListener = new NetworkListener(ip, 41850);

	SOCKET serverSocket = networkListener->getSocket();
	int retValue = networkListener->getRetValue();


	system("pause");
}*/



void NetworkListener::clientProc(SOCKET client, SOCKADDR_IN paramAddr)
{
	cout << "Client Conneced IP : " << inet_ntoa(paramAddr.sin_addr) << endl;

	char buf[BUFFER_SIZE + 1];

	while (true)
	{
		int len = recv(client, buf, BUFFER_SIZE, 0);
		if (len == SOCKET_ERROR || len == 0) {
			break;
		}

		buf[BUFFER_SIZE] = '\0';

		//printf("[TCP/%s:%d] %s\n", inet_ntoa(paramAddr.sin_addr), ntohs(paramAddr.sin_port), buf);
	}

	//select()

	cout << "DisConnected IP : " << inet_ntoa(paramAddr.sin_addr) << endl;
}

void NetworkListener::networkProc(SOCKET serverSocket, SOCKADDR_IN paramAddr)
{
	while (true)
	{
		cout << "Client Connect Wating..." << endl;
		int size = sizeof(paramAddr);
		SOCKET client = accept(serverSocket, (SOCKADDR*)&paramAddr, &size);

		thread clientThread([&]() {clientProc(client, paramAddr); });
		clientThread.join();
	}
}

int NetworkListener::getRetValue()
{
	return this->retValue;
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

	initSocketAddr(FALSE);

	Log::printLog("Network Listener Bind and Listen...");
	
	if ((retValue = ::bind(paramSocket, (SOCKADDR*)&this->serverAddr, sizeof(this->serverAddr))) == SOCKET_ERROR)
		nResult = 0;

	if ((retValue = listen(paramSocket, BACKLOG_COUNT)) == SOCKET_ERROR)
		nResult = 0;

	// non blocking mode
	ULONG mode = TRUE;
	if ((retValue = ioctlsocket(paramSocket, FIONBIO, &mode)) == SOCKET_ERROR)
		nResult = 0;


	paramRetValue = retValue;
	return nResult;
}

void NetworkListener::initSocketAddr(BOOLEAN IPv6)
{
	// Socket Init
	if (IPv6)
		this->serverAddr.sin_family = AF_INET6; // IPv4
	else
		this->serverAddr.sin_family = AF_INET; // IPv6
	this->serverAddr.sin_port = htons(this->port); // Port
	this->serverAddr.sin_addr.S_un.S_addr = inet_addr(this->ip); // Broadcast
}

int NetworkListener::Disconnect(SOCKET socket)
{
	if (closesocket(socket) == SOCKET_ERROR) // if function success return value is 0
		return 0;

	return 1;
}

TCHAR * NetworkListener::getIp()
{
	return this->ip;
}

USHORT NetworkListener::getPort()
{
	return this->port;
}

SOCKET NetworkListener::getSocket()
{
	return this->serverSocket;
}

SOCKADDR_IN NetworkListener::getSockAddr()
{
	return this->serverAddr;
}



// select model Server
/*
	
	FD_SET readSet;
	FD_SET writeSet;
	SOCKET clientSocket;
	SOCKADDR_IN clientAddr;
	int nAddrLength;

	ClientManager clientManager;

	while (TRUE)
	{
		// 소켓 셋 초기화
		FD_ZERO(&readSet);
		FD_ZERO(&writeSet);

		// 소켓 셋 지정
		FD_SET(serverSocket, &readSet);

		for (int i = 0; i < clientManager.getCount(); ++i)
		{
			Client *clientValue = clientManager.getValue(clientAddr);
			if (clientValue != nullptr)
			{
				if (clientValue->recvBytes > clientValue->sendBytes)
					FD_SET(clientValue->socket, &writeSet);
				else
					FD_SET(clientValue->socket, &readSet);
			}
		}

		// select()
		if ((retValue = select(0, &readSet, &writeSet, NULL, NULL)) == SOCKET_ERROR)
		{
			return 0;
		}

		// 소켓 셋 검사 #1 : 클라이언트 접속
		if (FD_ISSET(serverSocket, &readSet))
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
			Client *client = clientManager.getValue(clientAddr);
			if (FD_ISSET(client->socket, &readSet))
			{
				// 데이터받기
				retValue = recv(client->socket, client->recvBuffer, BUFFER_SIZE, 0);
				if (retValue == SOCKET_ERROR || retValue == 0)
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{
						Log::printLog("Error : recv()");
						Client *clientRemove = clientManager.remove(clientAddr);

						if (clientRemove)
						{
							delete clientRemove;
							clientRemove = nullptr;
						}
						continue;
					}
				}
				else
				{
					client->recvBytes = retValue;

					// 받은 데이터 출력
					SOCKADDR_IN sockAddr;
					int nAddrLength = sizeof(sockAddr);
					getpeername(client->socket, (SOCKADDR*)&sockAddr, &nAddrLength);

					client->recvBuffer[retValue] = '\0';
					printf("[TCP/%s:%d] %s\n", inet_ntoa(sockAddr.sin_addr), ntohs(sockAddr.sin_port), client->recvBuffer);
				}
			}

			if (FD_ISSET(client->socket, &writeSet))
			{
				// 데이터보내기
				retValue = send(client->socket, client->sendBuffer + client->sendBytes, client->recvBytes - client->sendBytes, 0);

				if (retValue == SOCKET_ERROR)
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{
						Log::printLog("Error : send()");
						Client *clientRemove = clientManager.remove(clientAddr);

						if (clientRemove)
						{
							delete clientRemove;
							clientRemove = nullptr;
						}
						continue;
					}
				}

				client->sendBytes += retValue;

				if (client->recvBytes == client->sendBytes)
					client->recvBytes = client->sendBytes = 0;
			}
		}
	}
*/