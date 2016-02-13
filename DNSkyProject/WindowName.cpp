#include <Windows.h>
#include "windowname.h"
#include <iostream>
#include <time.h>
#include <string>
#include "se.h"
#include "vlizer.h"
#include "Extras.h"


DWORD WINAPI setWindowThread(LPVOID)
{
#if !defined(CHN) && !defined(CHN_MSTERE)
	HWND hWnd;
	for (;;)
	{
		//龍之谷
		//#ifdef DEBUG
		hWnd = FindWindowW(L"DRAGONNEST", L"龍之谷");
		//#else
		//		hWnd = FindWindowW(L"SkyNest", L"SkyNest");
		//#endif

		if (hWnd != NULL)
		{
			char WTITLE[128];
#ifdef RO
			int version = getClientVer();
			wsprintf(WTITLE, enc("SkyNest REBORN v.%d"), version);
#endif
#ifdef RU
			wsprintf(WTITLE, enc("1stDN Russia"));
#endif
#ifdef AR
			wsprintf(WTITLE, enc("Arabic DragonNest - dnarabic.net"));
#endif
#ifdef VTM
			wsprintf(WTITLE, enc("Dragon Nest iTea"));
#endif
#ifdef CA
			wsprintf(WTITLE, enc("DragonNest"));
#endif
#ifdef THA
			wsprintf(WTITLE, enc("Easy DragonNest"));
#endif
			SetWindowTextA(hWnd, WTITLE);
			return NULL;
		}

		Sleep(1000);

	}
#endif
	return NULL;
}