#ifndef _THREADINFO_H_
#define _THREADINFO_H_

#include <thread>

using namespace std;

class ThreadInfo
{
private:
	pthread_cont_t mcond;
	pthread_mutex_t mmutex;
	int msocket;
public:
	ThreadInfo(pthread_cond_t, pthread_mutex_t);
	int TryLock();
	int Job();
	int UnLock();
};

#endif