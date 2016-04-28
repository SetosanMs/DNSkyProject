#include <string>
#include <Windows.h>
#include <time.h>
#include "Timer.h"
std::string getCurrentLocalDate() {
	// convert local time to string of form "YYYYMMDD"
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[10];

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), "%Y%m%d", timeinfo);
	return buffer;
}



DWORD WINAPI TimeCheckThread(LPVOID)
{
	std::string todaysDate;
	while (1)
	{
		todaysDate = getCurrentLocalDate();
		if (todaysDate > "20160407" || todaysDate < "20160307")
		{
			ExitProcess(0);
		}
		Sleep(30000);
	}
	return 0;
}