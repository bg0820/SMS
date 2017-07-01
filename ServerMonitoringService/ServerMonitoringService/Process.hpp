#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <Windows.h>
#include <Psapi.h>

#pragma comment(lib, "psapi.lib")

int Compare(const void * val1, const void * val2);

class Process
{
private:
	DWORD pid;
	TCHAR* name;
	TCHAR* path;
	TCHAR* commandLine;
	int threadCount;
	int processCount;
	DWORD memoryVal;
	double cpuVal;
	long ioRead;
	long ioWrite;
	long networkSendByte;
	long networkReceiveByte;
public:
	Process(DWORD pid = 0, TCHAR* name = NULL, TCHAR* commandLine = NULL, int threadCount = 0, int processCount = 0, DWORD memoryVal = 0, double cpuVal = 0.0, long ioRead = 0, long ioWrite = 0, long networkSendByte = 0, long networkReceiveByte = 0)
	{
		Process::pid = pid;
		Process::name = name;
		Process::path = path;
		Process::commandLine = commandLine;
		Process::threadCount = threadCount;
		Process::processCount = processCount;
		Process::memoryVal = memoryVal;
		Process::cpuVal = cpuVal;
		Process::ioRead = ioRead;
		Process::ioWrite = ioWrite;
		Process::networkSendByte = networkSendByte;
		Process::networkReceiveByte = networkReceiveByte;
	}

	~Process()
	{
		delete name;
		delete path;
		delete commandLine;
	}

#pragma region GET/SET
	DWORD getPID();
	TCHAR* getName();
	TCHAR* getPath();
	TCHAR* getCommandLine();
	int getThreadCount();
	int getProcessCount();
	DWORD getMemoryUsage();
	double getCpuUsage();
	long getIORead();
	long getIOWrite();
	long getNetworkSendByte();
	long getNetworkReceiveByte();

	void setPid(DWORD val);
	void setName(TCHAR * val);
	void setPath(TCHAR * val);
	void setCommandLine(TCHAR * val);
	void setThreadCount(int val);
	void setProcessCount(int val);
	void setMemoryUsage(DWORD val);
	void setCpuUsage(double val);
	void setIORead(long val);
	void setIOWrite(long val);
	void setNetworkSendByte(long val);
	void setNetworkReceiveByte(long val);
#pragma endregion

	const TCHAR* getName(const DWORD processId); // i��° ���μ��� �̸� ��ȯ
	const TCHAR* getPath(const DWORD processId); // i���� ���μ��� ��� ��ȯ
	const TCHAR* getCommandLine(const DWORD processId); // i��° ���μ��� Ŀ�ǵ���� ��ȯ
	int getNetworkUsage(const DWORD processId, double &val); // i��° ���μ��� ��Ʈ��ũ ��뷮 ��ȯ
	int getMemoryUsage(const DWORD processId, DWORD &val); // i��° ���μ��� �޸� ��뷮 ��ȯ
	int getCpuUsage(const DWORD processId, double &val); // i��° ���μ��� cpu ��뷮 ��ȯ
};
#endif