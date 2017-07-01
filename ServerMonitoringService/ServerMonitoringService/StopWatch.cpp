#include "StopWatch.hpp"

void StopWatch::Start()
{
	start = high_resolution_clock::now();
}

void StopWatch::Stop()
{
	end = high_resolution_clock::now();
}

long long StopWatch::getMs()
{
	return  (end - start).count() / 1000000.0;
}

void StopWatch::nanoPrint()
{
	cout << (end - start).count() << "ns" << endl;
}

void StopWatch::microPrint()
{
	cout << (end - start).count() / 1000.0 << "us" << endl;
}

void StopWatch::milliPrint()
{
	cout << (end - start).count() / 1000000.0 << "ms" << endl;
}

void StopWatch::secPrint()
{
	cout << (end - start).count() / 1000000000.0 << "sec" << endl;
}