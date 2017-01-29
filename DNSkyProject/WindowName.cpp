#include <Windows.h>
#include "windowname.h"
#include <iostream>
#include <time.h>
#include <string>
#include "se.h"
#include "vlizer.h"
#include "Extras.h"

#pragma warning(push)
#pragma warning(disable:4005)
#pragma warning(disable:4200)
#pragma warning(pop)

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
			wchar_t WTITLE[128];
#ifdef RO
			int version = getClientVer();
			wsprintfW(WTITLE, L"SkyNest REBORN v.%d", version);
#endif
#ifdef RU
		//	wsprintfW(WTITLE, L"1stDN Russia");
			wsprintfW(WTITLE, L"Dragon Nest");
#endif
#ifdef AR
			wsprintfW(WTITLE, L"Arabic DragonNest - dnarabic.net");
#endif
#ifdef VTM
			wsprintfW(WTITLE, L"Dragon Nest iTea");
#endif
#ifdef CA
			wsprintfW(WTITLE, L"DragonNest");
#endif
#ifdef THA
			wsprintfW(WTITLE, L"Easy DragonNest");
			//wsprintfW(WTITLE, L"Yelp DragonNest");
			//wsprintfW(WTITLE, L"Ice DragonNest");

#endif
#ifdef CHN_SRC
			wsprintfW(WTITLE, L"梦之谷");
#endif
#ifdef CHN_TNT
			wsprintfW(WTITLE, L"龙之谷");
#endif

			SetWindowTextW(hWnd, WTITLE);
			return NULL;
		}

		Sleep(1000);

	}
#endif
	return NULL;
}