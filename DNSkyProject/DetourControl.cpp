#include <Windows.h>
#include "DetourControl.h"
#include "Detours.h"
#include "dconnect.h"
#include "dreadfile.h"
#include "doutputstring.h"
#include "se.h"

void startDetours()
{
	DetourRestoreAfterWith();
	/* Socket Connect */
	dconnect = (int (WINAPI *)(SOCKET s, const struct sockaddr *name, int namelen))DetourFindFunction(enc("Ws2_32.dll"), enc("connect"));
	DetourAndErrorCheck(&(PVOID&)dconnect, myconnect, enc("connect"));

	/* Read File */
	dReadFile = (BOOL(WINAPI *)(HANDLE hFile, LPVOID lpBuffer, DWORD  nNumberOfBytesToRead, LPDWORD lpNumberOfByte, LPOVERLAPPED lpOverlapped))DetourFindFunction(enc("Kernel32.dll"), enc("ReadFile"));
	DetourAndErrorCheck(&(PVOID&)dReadFile, myReadFile, enc("ReadFile"));
	/* My Debug String */
#ifdef DEBUG
	dOutputDebugString = (void(WINAPI *)(LPCSTR lpOutputString))DetourFindFunction(enc("Kernel32.dll"), enc("OutputDebugStringA"));
	DetourAndErrorCheck(&(PVOID&)dOutputDebugString, myOutputDebugString, enc("OutputDebugStringA"));
#endif
}

void destroyDetours()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	CheckAndDetach(&(PVOID&)dconnect, myconnect, enc("connect"));
	CheckAndDetach(&(PVOID&)dReadFile, myReadFile, enc("ReadFile"));
	CheckAndDetach(&(PVOID&)dOutputDebugString, myOutputDebugString, enc("OutputDebugStringA"));
}
