#include <Windows.h>
#include <time.h>
#include <string>
#include "Misc.h"
#include "Memory.h"
#include "se.h"
#include "GameValidation.h"

std::string AccountName;
std::string IPAddr;
bool LoadNewUI = FALSE;

void generateRandomMutex()
{
#ifdef BUILD_98
	return;
#endif

	const char *randomMutex = enc("skyFUCKINGnest123456789");

	for (int i = 0; i < 8; i += 2)
	{
		srand((DWORD)time(NULL));
		int index1 = rand() % 22;
		SetByte((PVOID)(0x00D5A0EC + i), randomMutex[index1]);
	}
}

void checkCommandLine()
{

	LPWSTR *szArglist;
	int nArgs = 0;
	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
#if !defined(THA) && !defined(RU)
	if (nArgs < 5)
	{
#if defined(CHN) || defined(CHN_MSTERE) || defined(CHN_TNT)
		MessageBoxW(NULL, L"从开始的DNLauncher.exe", L"Game", MB_ICONWARNING);
#else
		MessageBoxA(NULL, enc("Start game from DNLauncher.exe!"), enc("Wait a sec!"), MB_ICONMASK);
#endif
		ExitProcess(0);
	}
#endif

	std::wstring AccountPortion(GetCommandLineW());
	size_t start = AccountPortion.find(L"logintoken:");
	size_t end = AccountPortion.find(L"&");
	std::wstring tempAcc = AccountPortion.substr(start + 11, end-(start+11));// / 11 = logintoken.
	AccountName = std::string(tempAcc.begin(), tempAcc.end());



	std::wstring cmdline(GetCommandLineW());
	std::size_t found = cmdline.find(L"/newui");
	if (found != std::string::npos)
	{
		LoadNewUI = TRUE;
	}

	int i = 0;
	for (i = 0; i < nArgs; i++)
	{
		if (wcsstr(szArglist[i], L"/ip:") != NULL)
		{
			std::wstring tempIP = szArglist[i];
			std::wstring tempIP1 = tempIP.substr(tempIP.find(L"/ip:") + 4);
			std::wstring ip = tempIP1.substr(0,tempIP1.find(L" ") );
			std::string newIP(ip.begin(), ip.end());
			IPAddr = newIP;
			if (checkIP(ip) == false)
			{
				//MessageBoxW(NULL, ip.c_str(), L"IP NOK", MB_OK);
			}
		}
	}
}