#include <Windows.h>
#include <list>
#include "stdafx.h"
#include "se.h"


std::list<std::string> window;

void LoadWindowNameData()
{
	window = {
		enc("Immunity Debugger - [CPU]"),
		enc("Cheat Engine 6"),
		enc("OllyDbg - [CPU]"),
		enc("DragonShark 1.0.0.0"),
		enc("Cheat Engine 5.0"),
		enc("Cheat Engine 5.1"),
		enc("Cheat Engine 5.1.1"),
		enc("Cheat Engine 5.2"),
		enc("Cheat Engine 5.3"),
		enc("Cheat Engine 5.4"),
		enc("Cheat Engine 5.5"),
		enc("Cheat Engine 5.6"),
		enc("Cheat Engine 5.6.1"),
		enc("Cheat Engine 6.0"),
		enc("Cheat Engine 6.1"),
		enc("Cheat Engine 6.2"),
		enc("Cheat Engine 6.3"),
		enc("Cheat Engine 6.4"),
		enc("Cheat Engine 6.5"),
		enc("Cheat Engine"),
		enc("Sandiebox"),
		enc("Sandiebox Control"),
		enc("Run Sandboxed"),
		enc("Process Explorer"),
		enc("Process Explorer 11.33"),
		enc("ArtMoney SE v7.44"),
		enc("ArtMoney SE v8.00"),
		enc("ArtMoney SE v7.43"),
		enc("ArtMoney SE v7.42"),
		enc("ArtMoney SE v7.41"),
		enc("ArtMoney SE v7.40"),
		enc("ArtMoney SE v7.39"),
		enc("ArtMoney SE v7.38"),
		enc("ArtMoney SE v7.37"),
		enc("ArtMoney SE v7.36"),
		enc("ArtMoney SE v7.35"),
		enc("ArtMoney SE v7.34"),
		enc("ArtMoney SE v7.33"),
		enc("ArtMoney SE v7.32"),
		enc("ArtMoney SE v7.31"),
		enc("Art*Mo*ney"),
	};
}

bool TitleWindow(LPCSTR WindowTitle){
	//VIRTUALIZER_DOLPHIN_WHITE_START
	HWND WinTitle = FindWindowA(NULL, WindowTitle);
	if (WinTitle > 0){
		SendMessage(WinTitle, WM_CLOSE, 0, 0);
		return false;
	}
	//VIRTUALIZER_DOLPHIN_WHITE_END
	return true;
}


void doWindowScan()
{
	for (std::list<std::string>::iterator it = window.begin(); it != window.end(); it++)
	{
		TitleWindow((*it).c_str()); 
	}
}