#include "Util.hpp"

string Util::currentDateTime()
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	string str = Util::string_format("[%04d-%02d-%02d %02d:%02d:%02d.%03d]", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	return str;
}

string Util::string_format(const string fmt_str, ...)
{
	int final_n, n = ((int)fmt_str.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
	string str;
	unique_ptr<char[]> formatted;
	va_list ap;
	while (1) {
		formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
		strcpy(&formatted[0], fmt_str.c_str());
		va_start(ap, fmt_str);
		final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
		va_end(ap);
		if (final_n < 0 || final_n >= n)
			n += abs(final_n - n + 1);
		else
			break;
	}
	return string(formatted.get());
}

string Util::getLocalIp()
{
	string ip;
	WSADATA wsaData;
	char name[255];
	PHOSTENT hostinfo;
	WORD DllVersion = MAKEWORD(2, 2);
	if (WSAStartup(DllVersion, &wsaData) == 0)
	{
		if (gethostname(name, sizeof(name)) == 0)
		{
			if ((hostinfo = gethostbyname(name)) != NULL)
			{
				ip = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
			}
		}
		WSACleanup();
	}

	return ip;
}

string Util::getStringToAddr(SOCKADDR_IN paramAddr)
{
	return Util::string_format("%s:%d", inet_ntoa(paramAddr.sin_addr), ntohs(paramAddr.sin_port));
}