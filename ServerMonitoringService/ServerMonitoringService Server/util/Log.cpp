#include "Log.hpp"

void Log::printErrLine(wchar_t * msg, int lineNumber)
{
	printf("%s : %d", msg, lineNumber);
}

void Log::errQuit(TCHAR* title, TCHAR *context)
{
	//LPVOID lpMsgBuf;

	/*FormatMessage(
	FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
	NULL, WSAGetLastError(),
	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	(LPTSTR)&lpMsgBuf, 0, NULL);*/
	MessageBox(NULL, (LPCTSTR)context, title, MB_ICONERROR);
	//LocalFree(lpMsgBuf);
	exit(-1);
}