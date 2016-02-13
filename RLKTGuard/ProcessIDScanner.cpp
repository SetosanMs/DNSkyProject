#include "ProcessIDScanner.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <list>
#include "se.h"
#include "Security.h"
#include "stdafx.h"

std::list<std::string> process;

void LoadProcessNameData()
{
	process = { 
		enc("cheatengine-i386.exe"),
		enc("cheatengine-x86_64.exe"),
		enc("ollydbg.exe"),
		enc("wpepro.net.exe"),
		enc("x64_dbg.exe"),
		enc("x32_dbg.exe"),
		enc("x96_dbg.exe"),
		enc("procexp64.exe"),
		enc("procexp.exe"),
		enc("immunitydebugger.exe"),
		enc("petools.exe"),
		enc("processhacker.exe")
	};
}


void ScanProcId(const char* ProcName){
	PROCESSENTRY32 pe32;
	HANDLE hSnapshot = NULL;
	
	pe32.dwSize = sizeof(PROCESSENTRY32);
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (Process32First(hSnapshot, &pe32)){
		do{
			//VIRTUALIZER_DOLPHIN_WHITE_START
			//make it toLOWER!!@
			for (DWORD i = 0; i < strlen(pe32.szExeFile); ++i)
				pe32.szExeFile[i] = tolower(pe32.szExeFile[i]);

			if (strcmp(pe32.szExeFile, ProcName) == 0)
			{
				
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);  // Close detected process
				TerminateProcess(hProcess, NULL);
				Sleep(300);
				
				DWORD exitCode = 0;
				if (GetExitCodeProcess(hProcess, &exitCode) == FALSE)
				{
				}
				//VIRTUALIZER_DOLPHIN_WHITE_END
				if (exitCode != STILL_ACTIVE)
				{
#ifdef DEBUG
					MessageBoxA(NULL, "Antihack , process was successfully terminated.\nYou can play.", ProcName, MB_OK);
				//	Finalize_OpenPage();
#endif
					break;
				}
				else{
#ifdef DEBUG
					MessageBoxA(NULL, "Cannot kill hack process.", "Memory Scanner", MB_OK);
#endif
//					Finalize_OpenPage(pe32.szExeFile);
					//security s;
					//VIRTUALIZER_DOLPHIN_WHITE_START
//					s.crash();
					//VIRTUALIZER_DOLPHIN_WHITE_END
					ExitProcess(0);
				}
#ifdef DEBUG
				MessageBoxA(NULL, "[Scanner] Found process name", ProcName, MB_OK);
#endif
				
			}
		} while (Process32Next(hSnapshot, &pe32));
	}
	if (hSnapshot != INVALID_HANDLE_VALUE)
		CloseHandle(hSnapshot);
}


void doProcScan()
{

	for (std::list<std::string>::iterator it = process.begin(); it != process.end(); it++)
	{
		ScanProcId((*it).c_str()); //hack
		//Sleep(1000);
	}
}