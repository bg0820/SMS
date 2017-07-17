#include "NetworkListener.hpp"


DWORD NetworkListener::callbackProc(LPVOID hIOCP)
{
	NetworkListener * instance = reinterpret_cast <NetworkListener *> (hIOCP);

	HANDLE threadHandler = *((HANDLE *)hIOCP);
	DWORD receiveBytes;
	DWORD sendBytes;
	ULONG_PTR completionKey; // CreateIoCompletionPort() Call Value
	SOCKETINFO *eventSocket; // struct addr

	while (TRUE)
	{
		// iocpHandle, receive Byte, completionKey, struct Addr, waitTime ms
		// if io success packet exist, dequeu handle

		if (GetQueuedCompletionStatus(threadHandler, &receiveBytes, &completionKey, (LPOVERLAPPED*)&eventSocket, INFINITE) == 0) // faiied return value is zero
		{
			// get Client SOCKADDR To SOCKET
			SOCKADDR_IN clientAddr;
			int addrLength = sizeof(clientAddr);
			getpeername(eventSocket->socket, (SOCKADDR*)&clientAddr, &addrLength);

			Log::printLog(Util::format("Client DisConnected [%s], Count : %d", Util::getStringToAddr(clientAddr).c_str(), --clientCount));
			Log::printLog("Error - GetQueuedCompletionStatus Failure");
			closesocket(eventSocket->socket);

			if (eventSocket)
			{
				delete eventSocket;
				eventSocket = nullptr;
			}
			return 1;
		}

		// get Client SOCKADDR To SOCKET
		SOCKADDR_IN clientAddr;
		int addrLength = sizeof(clientAddr);
		getpeername(eventSocket->socket, (SOCKADDR*)&clientAddr, &addrLength);

		eventSocket->dataBuffer.len = receiveBytes;

		if (receiveBytes == 0)
		{
			closesocket(eventSocket->socket);

			if (eventSocket)
			{
				delete eventSocket;
				eventSocket = nullptr;
			}

			continue;
		}
		else
		{
			if (WSASend(eventSocket->socket, &(eventSocket->dataBuffer), 1, &sendBytes, 0, NULL, NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
					Log::printLog(Util::format("Client DisConnected [%s], Count : %d", Util::getStringToAddr(clientAddr).c_str(), --clientCount));
					Log::printLog(Util::format("Error - Fail WSASend(error_code : %d)", WSAGetLastError()));

					closesocket(eventSocket->socket);

					if (eventSocket)
					{
						delete eventSocket;
						eventSocket = nullptr;
					}
				}
			}

			if (eventSocket)
			{
				memset(eventSocket->messageBuffer, 0x00, BUFFER_SIZE);
				eventSocket->receiveBytes = 0;
				eventSocket->sendBytes = 0;
				eventSocket->dataBuffer.len = BUFFER_SIZE;
				eventSocket->dataBuffer.buf = eventSocket->messageBuffer;
			}
			DWORD flags = 0;

			if (WSARecv(eventSocket->socket, &(eventSocket->dataBuffer), 1, &receiveBytes, &flags, &eventSocket->overlapped, NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
					Log::printLog(Util::format("Client DisConnected [%s], Count : %d", Util::getStringToAddr(clientAddr).c_str(), --clientCount));
					Log::printLog(Util::format("Error - Fail WSARecv(error_code : %d)", WSAGetLastError()));
					closesocket(eventSocket->socket);

					if (eventSocket)
					{
						delete eventSocket;
						eventSocket = nullptr;
					}
				}
			}
		}
	}
}

int main()
{
	string localIp = Util::getLocalIp();
	Log::printLog(Util::format("Local IP : %s", localIp.c_str()));

	TCHAR ip[16];
	_tcscpy_s(ip, CA2T(localIp.c_str())); // string to TCHAR

	NetworkListener *networkListener = new NetworkListener(ip, 41850);

	SOCKET serverSocket = networkListener->getSocket();
	int retValue = networkListener->getRetValue();
	HANDLE hIOCP = networkListener->getHIOCP();
	networkListener->Start();


	Log::printLog("Server Socket Closed");
	closesocket(serverSocket);

	Log::printLog("WinSock End");
	WSACleanup();

	system("pause");
}


/*
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
}*/

void NetworkListener::Start()
{

	SOCKADDR_IN clientAddr;
	int addrLength = sizeof(SOCKADDR_IN);
	memset(&clientAddr, 0, addrLength);
	SOCKET clientSocket;
	SOCKETINFO *socketInfo;
	DWORD receiveBytes;
	DWORD flags;

	while (1)
	{
		if ((clientSocket = accept(serverSocket, (SOCKADDR *)&clientAddr, &addrLength)) == INVALID_SOCKET)
		{
			Log::printLog("Error - Accept Failure");
			// return 1;
		}

		// TODO : CLIENT OBJECT CREATE
		socketInfo = new SOCKETINFO; // (struct SOCKETINFO *)malloc(sizeof(struct SOCKETINFO));
		memset((void *)socketInfo, 0x00, sizeof(SOCKETINFO));
		socketInfo->socket = clientSocket;
		socketInfo->receiveBytes = 0;
		socketInfo->sendBytes = 0;
		socketInfo->dataBuffer.len = BUFFER_SIZE;
		socketInfo->dataBuffer.buf = socketInfo->messageBuffer;
		flags = 0;

		Log::printLog(Util::format("Client Connected [%s], Count : %d", Util::getStringToAddr(clientAddr).c_str(), ++clientCount));
		hIOCP = CreateIoCompletionPort((HANDLE)clientSocket, hIOCP, (DWORD)socketInfo, 0);

		// 중첩 소캣을 지정하고 완료시 실행될 함수를 넘겨준다.
		if (WSARecv(socketInfo->socket, &socketInfo->dataBuffer, 1, &receiveBytes, &flags, &(socketInfo->overlapped), NULL))
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				Log::printLog(Util::format("Client DisConnected [%s], Count : %d", Util::getStringToAddr(clientAddr).c_str(), --clientCount));
				Log::printLog("Error - IO pending Failure");
				// return 1;
			}
		}
	}
}

int NetworkListener::getRetValue()
{
	return this->retValue;
}


int NetworkListener::Init(SOCKET &paramSocket, SOCKADDR_IN &parmAddr, int &paramRetValue, HANDLE &handle)
{
	int nResult = 1;
	int retValue;
	Log::printLog("Network Listener Socket Initializing...");

	//WinSock Init
	if (WSAStartup(DllVersion, &wsa) != 0)
	{
		Log::printLog("Error - Can not load 'winsock.dll' file");
		nResult = 0;
	}

	// Socket Create
	paramSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (paramSocket == INVALID_SOCKET) // FAILED
	{
		Log::printLog("Error - Invalid socket");
		nResult = 0;
	}

	Log::printLog(Util::format("Network Listener Socket Address [%s:%d] Initializing...", this->serverIp, this->serverPort));
	initSocketAddr(FALSE);

	Log::printLog("Network Listener Bind and Listen...");
	if ((retValue = ::bind(paramSocket, (SOCKADDR*)&this->serverAddr, sizeof(this->serverAddr))) == SOCKET_ERROR)
		nResult = 0;

	if ((retValue = listen(paramSocket, BACKLOG_COUNT)) == SOCKET_ERROR)
		nResult = 0;


	if (nResult == 0)
	{
		// Socket Closed
		closesocket(paramSocket);
		// Winsock End
		WSACleanup();
	}

	HANDLE hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	ULONG threadId;

	// Create Thread
	for (int i = 0; i < this->threadCount; i++)
	{
		// 
		Thread *thread = new Thread(&NetworkListener::callbackProc, &hIOCP);
		//hThread[i] = CreateThread(NULL, 0, std::bind(&NetworkListener::threadProc, this), &hIOCP, 0, &threadId);
	}

	handle = hIOCP;
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
	this->serverAddr.sin_port = htons(this->serverPort); // Port
	this->serverAddr.sin_addr.S_un.S_addr = inet_addr(this->serverIp); // Broadcast
}

int NetworkListener::Disconnect(SOCKET socket)
{
	if (closesocket(socket) == SOCKET_ERROR) // if function success return value is 0
		return 0;

	return 1;
}

TCHAR * NetworkListener::getIp()
{
	return this->serverIp;
}

USHORT NetworkListener::getPort()
{
	return this->serverPort;
}

SOCKET NetworkListener::getSocket()
{
	return this->serverSocket;
}

HANDLE NetworkListener::getHIOCP()
{
	return this->hIOCP;
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