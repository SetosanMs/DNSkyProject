#include "Security.h"
#include <Windows.h>
#include <stdio.h>
#include <ntdll.h>
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include "Extras.h"
#include "se.h"
#include "CRC32.h"


#ifdef RO
#define EXECUTABLE_CRC 0x9BE87391
#elif CHN
#define EXECUTABLE_CRC 0x72A08A50
#elif CHN_MSTERE
#define EXECUTABLE_CRC 0x9b8b16ea // 0xa7aa430f //dbg: 0x9b8b16ea
#else
#define EXECUTABLE_CRC 0xFFFFFFFF
#endif

security s;

security::security()
{
}


security::~security()
{
}
	

void security::crash()
{
	_asm{
		mov eax, 0x1337;
		call eax;
	}
}


void security::hideModule()
{

#ifdef _WIN64
	PPEB_VISTA_7 pPEB = (PPEB_VISTA_7)__readgsqword(0x60);
	//LPWSTR captionMsg = L"64-bit Application";
#else
	PPEB_VISTA_7 pPEB = (PPEB_VISTA_7)__readfsdword(0x30);
	//LPWSTR captionMsg = L"32-bit Application";
#endif
	wchar_t mainMsg[MAX_PATH] = { 0 };
	PLIST_ENTRY pListEntry = pPEB->LoaderData->InLoadOrderModuleList.Flink;
	PLDR_DATA_TABLE_ENTRY_7 pModule = NULL;

	//LPWSTR finishedMainMsg = L"No match found!";

	while (pListEntry != &pPEB->LoaderData->InLoadOrderModuleList && pListEntry != NULL) {

		pModule = (PLDR_DATA_TABLE_ENTRY_7)pListEntry->Flink;
		if (pModule->BaseDllName.Buffer != NULL)
#ifdef AR
		if (wcsncmp(pModule->BaseDllName.Buffer, L"cap80.dll", 0x12) == 0) {
#elif CHN_MSTERE
		if (wcsncmp(pModule->BaseDllName.Buffer, L"mstere.dll", 0x13) == 0) {
#elif THA
		if (wcsncmp(pModule->BaseDllName.Buffer, L"EasyCore.dll", 0x15) == 0) {
#else
		if (wcsncmp(pModule->BaseDllName.Buffer, L"dnsky.dll", 0x12) == 0) {
#endif

				/*_snwprintf(
				mainMsg,
				MAX_PATH * 2,
				L"Job done! Found match.\nNow,"
				L" scan my loaded Modules and try to see kernel32.dll.\n\n"
				L"ModuleName: %ws\n[-]BaseAddress: 0x%p\n[-]EntryPoint: 0x%p\n"
				L"[-]SizeOfImage: 0x%p\n\n[by stigma from I3CT]\nInsid3Code Team",
				pModule->BaseDllName.Buffer,
				pModule->BaseAddress,
				pModule->EntryPoint,
				pModule->SizeOfImage);
				*/
			pModule->InLoadOrderModuleList.Flink->Blink = pModule->InLoadOrderModuleList.Blink;
			pModule->InLoadOrderModuleList.Blink->Flink = pModule->InLoadOrderModuleList.Flink;

			pModule->InMemoryOrderModuleList.Flink->Blink = pModule->InMemoryOrderModuleList.Blink;
			pModule->InMemoryOrderModuleList.Blink->Flink = pModule->InMemoryOrderModuleList.Flink;

			pModule->InInitializationOrderModuleList.Flink->Blink = pModule->InInitializationOrderModuleList.Blink;
			pModule->InInitializationOrderModuleList.Blink->Flink = pModule->InInitializationOrderModuleList.Flink;

		//	MessageBoxW(NULL, mainMsg, captionMsg, MB_ICONINFORMATION);
		//	finishedMainMsg = L"Finished!";
			break;
		}

		pListEntry = pListEntry->Flink;
	}

}

void security::IntegrityCheck()
{
	TCHAR szEXEPath[2048];
	GetModuleFileName(NULL, szEXEPath, 2048);
	std::string name = getFileName(szEXEPath);

#ifdef RO
	std::string compareTo = enc("dragon.exe");
#else
	std::string compareTo = enc("dragonnest.exe");
#endif

	std::transform(name.begin(), name.end(), name.begin(), ::tolower);

	CCRC32 MyCRC32;
	MyCRC32.Initialize();
	unsigned long CRC = MyCRC32.FileCRC(name.c_str());

	if (name.compare(compareTo) != 0 || CRC != EXECUTABLE_CRC)
	{
		this->crash();
		ExitProcess(0);
	}

}