#include <Windows.h>
#include <TlHelp32.h>
#include <list>
#include <string>
#include <map>
#include "se.h"

std::list<std::string> module;
HANDLE hSnapshotStart = NULL;
std::list<std::string> SnapshotModule;

//list
void LoadModuleList()
{
	module = {
		enc("edn.dll"),
	};
}


//START
void TakeModuleSnapShoot()
{
	MODULEENTRY32 pe32;
	pe32.dwSize = sizeof(MODULEENTRY32);

	hSnapshotStart = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	if (Module32First(hSnapshotStart, &pe32)){
		do{
			//printf("Loaded Module : %s \n", pe32.szModule);// pe32.szModule
			SnapshotModule.push_back(pe32.szModule);
		} while (Module32Next(hSnapshotStart, &pe32));
	}
}


//scanner
void ScanModule()
{
	MODULEENTRY32 pe32;
	pe32.dwSize = sizeof(MODULEENTRY32);

	hSnapshotStart = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	if (Module32First(hSnapshotStart, &pe32)){
		do{
			//printf("Loaded Module : %s \n", pe32.szModule);// pe32.szModule
			//Check 1 - compare with blacklisted :)
		//	if (std::find(SnapshotModule.begin(), SnapshotModule.end(), pe32.szModule) == SnapshotModule.end())
		///		MessageBox(NULL, "Cannot FIND module !!", pe32.szModule, MB_OK);
			//Check 2 - compare with started process snapshoot.
			if (std::find(SnapshotModule.begin(), SnapshotModule.end(), pe32.szModule) == SnapshotModule.end())
				MessageBox(NULL, "Cannot FIND module !!", pe32.szModule, MB_OK);

		} while (Module32Next(hSnapshotStart, &pe32));
	}
}

//Do scan
void doModuleScan()
{
	ScanModule();
}