#include "Util.hpp"

string Util::currentDateTime()
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	string str = Util::format("[%04d-%02d-%02d %02d:%02d:%02d.%03d]", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	return str;
}

void Util::stringReplace(string& subject, const string& search, const string& replace)
{
	size_t pos = 0;

	while ((pos = subject.find(search, pos)) != string::npos)
	{
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
}

string Util::format(const string fmt_str, ...)
{
	int final_n, n = ((int)fmt_str.size()) * 2;
	string str;
	unique_ptr<char[]> formatted;
	va_list ap;
	while (1) {
		formatted.reset(new char[n]);
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
	return Util::format("%s:%d", inet_ntoa(paramAddr.sin_addr), ntohs(paramAddr.sin_port));
}

void Util::microDelay(ULONG time)
{
	time_point<steady_clock> start = high_resolution_clock::now();
	while ((high_resolution_clock::now() - start).count() / 1000.0 < time);
}

string Util::getTimeString(tm t)
{
	string str = Util::format("%04d-%02d-%02d %02d:%02d:%02d",
		t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);

	return str;
}

int Util::Compress(const TCHAR *paramRawData, int &paramSrcSize, TCHAR *&paramCompressedData, int &paramCompressSize)
{
	// 마지막 NULL 문자
	// Last null char
	const int srcSize = (int)(strlen(paramRawData) + 1);
	// 압축 된 출력의 최대 크기를 반환
	const int maxCompressSize = LZ4_compressBound(srcSize);
	// We will use that size for our destination boundary when allocating space.
	TCHAR *compressedData = new TCHAR[maxCompressSize];

	if (compressedData == NULL)
		return -1; // Failed to allocate memory for *compressed_data.

	const int compressedSize = LZ4_compress_default(paramRawData, compressedData, srcSize, maxCompressSize);

	// Check return_value to determine what happened.
	if (compressedSize < 0)
		return -2; // A negative result from LZ4_compress_default indicates a failure trying to compress the data.  See exit code (echo $?) for value returned.
	else if (compressedSize == 0)
		return -3; // A result of 0 means compression worked, but was stopped because the destination buffer couldn't hold all the information.

	paramSrcSize = srcSize;
	paramCompressedData = compressedData;
	paramCompressSize = compressedSize;

	return 1;
}

int Util::DeCompress(const TCHAR *paramCompressedData, int paramCompressedDataSize, int paramRawSize, TCHAR *&paramRawData)
{
	TCHAR *decompressData = new TCHAR[paramRawSize];

	if (decompressData == NULL)
		return -1;

	const int decompressedSize = LZ4_decompress_safe(paramCompressedData, decompressData, paramCompressedDataSize, paramRawSize);

	/*
	no longer useful
	이제 필요 없음
	*/
	if (paramCompressedData)
	{
		delete[] paramCompressedData;
		paramCompressedData = nullptr;
	}

	if (decompressedSize < 0)
		return -2; // A negative result from LZ4_decompress_safe indicates a failure trying to decompress the data.  See exit code (echo $?) for value returned.
	else if (decompressedSize == 0)
		return -3; // I'm not sure this function can ever return 0.  Documentation in lz4.h doesn't indicate so.


	paramRawData = decompressData;
	return 1;
}