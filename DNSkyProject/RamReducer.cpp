#include <Windows.h>
#include <Psapi.h>
#include "RamReducer.h"

DWORD __stdcall ReduceRAMThread(LPVOID) {

	while (1) {
		EmptyWorkingSet(GetCurrentProcess());
		Sleep(10000);
	}

	return NULL;
}