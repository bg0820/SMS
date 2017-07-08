#include "Log.hpp"

void Log::printErrLine(wchar_t * msg, int lineNumber)
{
	printf("%s : %d", msg, lineNumber);
}
