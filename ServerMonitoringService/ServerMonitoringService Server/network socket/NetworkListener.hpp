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

};

#endif