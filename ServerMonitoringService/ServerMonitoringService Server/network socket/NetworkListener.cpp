#include "NetworkListener.hpp"

int NetworkListener::Init(SOCKET &parmSocket)
{
	//WinSock Init
	if (WSAStartup(DllVersion, &wsa) != 0)
		return 0;

	// Socket Create
	parmSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (parmSocket == INVALID_SOCKET) // FAILED
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
	if (listen(socket, SOMAXCONN) == SOCKET_ERROR)
		return 0;

	return 1;
}

void NetworkListener::initSocketAddr(SOCKADDR_IN parmAddr, u_short port, const TCHAR *ip)
{
	// Socket Init
	addr.sin_family = AF_INET; // IPv4
	addr.sin_port = htons(port); // Port
	addr.sin_addr.S_un.S_addr = inet_addr(ip); // Broadcast
	parmAddr = addr;
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


void clientProc(SOCKET client, SOCKADDR_IN parmAddr)
{
	cout<<"Client Conneced IP : " << inet_ntoa(parmAddr.sin_addr) << endl;
}

void networkProc(SOCKET serverSocket, SOCKADDR_IN parmAddr)
{
	while (true)
	{
		cout << "Client Connect Wating..." << endl;
		int size = sizeof(parmAddr);
		SOCKET client = accept(serverSocket, (SOCKADDR*)&parmAddr, &size);

		thread clientThread(&clientProc, client, parmAddr);
		clientThread.join();
	}
}

int main()
{
	cout << "Server Start" << endl;
	cout << "Network Listener..." << endl;
	NetworkListener networkListener;
	//NetworkListener *networkListener = new NetworkListener;

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
	thread networkThread(&networkProc, socket, addr);
	networkThread.join();

	system("pause");
}