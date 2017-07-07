#include <iostream>
#include "network socket/NetworkManager.hpp"

using namespace std;


int main()
{
	cout << "Server Start" << endl;
	cout << "Network Listener..." << endl;

	NetworkManager *networkManager = new NetworkManager();
	networkManager->Init();
	networkManager->Start();

}