#include <Windows.h>
#include <list>
#include "se.h"
#include "vlizer.h"
#include "HTTPguard.h"

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

std::list<std::wstring> window;
std::list<std::wstring> windowclass;

void LoadWindowNameData()
{
	VIRTUALIZER_TIGER_WHITE_START
	window = {
		L"Immunity Debugger - [CPU]",
		L"Cheat Engine 6",
		L"OllyDbg - [CPU]",
		L"DragonShark 1.0.0.0",
		L"Cheat Engine 5.0",
		L"Cheat Engine 5.1",
		L"Cheat Engine 5.1.1",
		L"Cheat Engine 5.2",
		L"Cheat Engine 5.3",
		L"Cheat Engine 5.4",
		L"Cheat Engine 5.5",
		L"Cheat Engine 5.6",
		L"Cheat Engine 5.6.1",
		L"Cheat Engine 6.0",
		L"Cheat Engine 6.1",
		L"Cheat Engine 6.2",
		L"Cheat Engine 6.3",
		L"Cheat Engine 6.4",
		L"Cheat Engine 6.5",
		L"Cheat Engine",
		L"Sandiebox",
		L"Sandiebox Control",
		L"Run Sandboxed",
		L"Process Explorer",
		L"Process Explorer 11.33",
		L"ArtMoney SE v7.44",
		L"ArtMoney SE v8.00",
		L"ArtMoney SE v7.43",
		L"ArtMoney SE v7.42",
		L"ArtMoney SE v7.41",
		L"ArtMoney SE v7.40",
		L"ArtMoney SE v7.39",
		L"ArtMoney SE v7.38",
		L"ArtMoney SE v7.37",
		L"ArtMoney SE v7.36",
		L"ArtMoney SE v7.35",
		L"ArtMoney SE v7.34",
		L"ArtMoney SE v7.33",
		L"ArtMoney SE v7.32",
		L"ArtMoney SE v7.31",
		L"Art*Mo*ney",
		L"OllyDbg",
		L"TCPView",
		L"Sysinternals",
		//alea chinezesti
#if !defined(CHN_MSTERE)
		L"用户登陆",
		L"私服版启动界面(验证通过，可以使用)_",
		L"http://lzgxx.com",
		L"13008655209",
		L"正在载入游戏IP"
#endif
	};
	VIRTUALIZER_TIGER_WHITE_END
}

void LoadWindowClassData()
{
	VIRTUALIZER_TIGER_WHITE_START
		windowclass = {
#if !defined(CHN_MSTERE)
		L"DNXXGPKGOGO",
		L"WTWindow",
		L"DNXXGPK",
		L"DNXX",
#endif
	};
	VIRTUALIZER_TIGER_WHITE_END
}

bool TitleWindow(LPWSTR WindowTitle){
	//VIRTUALIZER_DOLPHIN_WHITE_START
	HWND WinTitle = FindWindowW(NULL, WindowTitle);
	if (WinTitle > 0){
		SendMessage(WinTitle, WM_CLOSE, 0, 0);
		return false;
	}
	//VIRTUALIZER_DOLPHIN_WHITE_END
	return true;
}


void doWindowScan()
{
	EnumWindows(EnumWindowsProc, NULL);

	/*for (std::list<std::wstring>::iterator it = window.begin(); it != window.end(); it++)
	{
		//TitleWindow((*it).c_str()); 
	}*/
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	wchar_t wclass[255];
	wchar_t title[255];

	GetWindowTextW(hwnd, title, sizeof(title));
	GetClassNameW(hwnd, wclass, sizeof(wclass));

	for (std::list<std::wstring>::iterator it = window.begin(); it != window.end(); it++)
	{
		if (wcsstr(title, (*it).c_str()) != 0)
		{
			SendMessage(hwnd, WM_CLOSE, 0, 0);
		}
	}

	for (std::list<std::wstring>::iterator it = windowclass.begin(); it != windowclass.end(); it++)
	{
		if (wcsstr(wclass, (*it).c_str()) != 0)
		{
			SendMessage(hwnd, WM_CLOSE, 0, 0);
		}
	}

	return TRUE;
}