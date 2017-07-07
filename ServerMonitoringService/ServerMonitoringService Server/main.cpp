#include <iostream>
#include <thread>

using namespace std;



int main()
{
	cout << "Server Start" << endl;

	thread networkThread(&networkProc);
	networkThread.join();
}