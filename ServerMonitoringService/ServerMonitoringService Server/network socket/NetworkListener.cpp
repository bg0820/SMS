#include "NetworkListener.hpp"

int NetworkListener::Init(SOCKET &paramSocket)
{
	//WinSock Init
	if (WSAStartup(DllVersion, &wsa) != 0)
		return 0;

	// Socket Create
	paramSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (paramSocket == INVALID_SOCKET) // FAILED
		return 0;

	return 1;
}

int NetworkListener::Bind(SOCKET socket)
{
	if (bind(socket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
		return 0;

	return 1;
}

int NetworkListener::Listen(SOCKET socket)
{
	if (listen(socket, BACKLOG_COUNT) == SOCKET_ERROR)
		return 0;

	return 1;
}

void NetworkListener::initSocketAddr(SOCKADDR_IN &paramAddr, u_short port, const TCHAR *ip)
{
	// Socket Init
	addr.sin_family = AF_INET; // IPv4
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

		printf("[TCP/%s:%d] %s\n", inet_ntoa(paramAddr.sin_addr), ntohs(paramAddr.sin_port), buf);
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

void NetworkListener::Start()
{
}

int main()
{
	cout << "Server Start" << endl;
	cout << "Network Listener..." << endl;
	NetworkListener networkListener;

	SOCKET socket;
	cout << "Network Listener Initializing..." << endl;
	if (!networkListener.Init(socket))
		return 0;

	SOCKADDR_IN addr = { 0 };
	cout << "Network Listener SocketAddr Initializing..." << endl;
	networkListener.initSocketAddr(addr, 8080, "127.0.0.1");

	cout << "Network Listener Bind and Listen..." << endl;
	networkListener.Bind(socket);
	networkListener.Listen(socket);

	cout << "Network Thread Create..." << endl;
	thread networkThread([&]() {networkListener.networkProc(socket, addr); });
	networkThread.join();

	system("pause");
}