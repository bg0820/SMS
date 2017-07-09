#include "Log.hpp"

// string str = Util::string_format("%s %d", "", int);
void Log::printLog(string msg)
{
	string time = Util::currentDateTime();
	cout << time << " " << msg << endl;
}

void Log::printErrLine(string file, int lineNumber, string msg)
{
	string time = Util::currentDateTime();
	cout << time << " " << file << ":" << lineNumber << " - " << msg << endl;
}

void Log::errQuit(string title, string context)
{
	//LPVOID lpMsgBuf;

	/*FormatMessage(
	FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
	NULL, WSAGetLastError(),
	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	(LPTSTR)&lpMsgBuf, 0, NULL);*/
	MessageBox(NULL, (LPCTSTR)context.c_str(), title.c_str(), MB_ICONERROR);
	//LocalFree(lpMsgBuf);
	exit(-1);
}