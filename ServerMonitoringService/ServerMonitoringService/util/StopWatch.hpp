#ifndef _STOPWATCH_H_
#define _STOPWATCH_H_

#include <iostream>
#include <time.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

class StopWatch
{
private:
	time_point<steady_clock> start;
	time_point<steady_clock> end;
public:
	void Start();
	void Stop();
	void nanoPrint();
	void microPrint();
	void milliPrint();
	void secPrint();
	long long getMs();
};

#endif