#include "Log.hpp"

// string str = Util::string_format("%s %d", "", int);
void Log::printLog(string msg)
{
	string time = Util::currentDateTime();
	//cout << "printLog() : " << time << "sizeof() : " << sizeof(time)<< endl;
	cout << time << " " << msg << endl;
}

void Log::printErrLine(string file, int lineNumber, string msg)
{
	string time = Util::currentDateTime();
	cout << time << " " << file << ":" << lineNumber << " - " << msg << endl;
}

void Log::showMsg(string title, string context)
{
	MessageBox(NULL, (LPCTSTR)context.c_str(), title.c_str(), MB_ICONERROR);
}