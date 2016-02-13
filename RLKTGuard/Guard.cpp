#include <Windows.h>
#include "Guard.h"
#include "ReSecurity.h"
#include "Security.h"
#include "MemoryScanner.h"
#include "ProcessIDScanner.h"
#include "WindowScanner.h"
#include "LoadedModuleScanner.h"
#include "stdafx.h"

DWORD tickCheck;

void GuardThread()
{
	////VIRTUALIZER_DOLPHIN_WHITE_START
	//VIRTUALIZER_DOLPHIN_WHITE_START
	LoadProcessNameData();
	LoadWindowNameData();
	//LoadModuleList();
	//TakeModuleSnapShoot();
	//VIRTUALIZER_DOLPHIN_WHITE_END
	do
	{
#ifdef DEBUG
#else
		//VIRTUALIZER_DOLPHIN_WHITE_START
		if (IsDebuggerPresent() || IsDbgPresentPrefixCheck() || Int2DCheck() || CheckProcessDebugFlags() || DebugObjectCheck() || CheckDbgPresentCloseHandle())
		{
			//VIRTUALIZER_DOLPHIN_WHITE_END

			//MessageBoxA(NULL, "Please close any window that may disrupt the game", "OK!", MB_ICONMASK);

			//VIRTUALIZER_DOLPHIN_WHITE_START
			//s.crash();
			//VIRTUALIZER_DOLPHIN_WHITE_END
			ExitProcess(0);

		}
#endif
#ifdef ANTIHACK
		MemoryScanner(); //Memory Scanner with memory footprint
		doProcScan(); //Executable name 
		doWindowScan(); //Window name scan
#endif
		//doModuleScan();
		tickCheck = GetTickCount();


		Sleep(2500);
	} while (1);
	////VIRTUALIZER_DOLPHIN_WHITE_END
}

void InitGuard()
{
	//VIRTUALIZER_DOLPHIN_WHITE_START

	CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(GuardThread), NULL, 0, 0);

	//VIRTUALIZER_DOLPHIN_WHITE_END
}

